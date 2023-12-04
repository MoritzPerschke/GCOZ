#include "DelayCalculator.h"

DelayCalculator::DelayCalculator() {
	baselineAdded = false;
}

void DelayCalculator::printBaseline() { // this probably won't be needed anymore
	std::cout <<
		"= ======= ======= ======= ======= ======= ======= ======= =======  ======= ======= ======= ======= ======= =" << std::endl <<
		"Baseline Method Durations:" << std::endl;
	for (int i = 0; i < _baselineDurationsAverage.size(); i++) {
		std::cout << std::setfill(' ') << std::setw(27) << methodNames[i] << ": " << _baselineDurationsAverage[i];
		if ((i + 1) % 3 == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << "\n" << "Profiled methods:\n";
	for (int i = 0; i < D3D11_METHOD_COUNT; i++) {
		if (_baselineDurationsAverage[i] > Nanoseconds(1000)) {
			std::cout << std::setfill(' ') << std::setw(25) << methodNames[i] << ": " << _baselineDurationsAverage[i];
			if ((i + 1) % 3 == 0) {
				std::cout << std::endl;
			}
		}
	}
	std::cout << std::endl <<
		"= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= =" << std::endl;
}

void DelayCalculator::addBaseline(std::queue<int>& _idCollectorMethodVector) {
	spdlog::info("Calculating baseline durations for methods");
	named_mutex _durationsMutex(open_only, "gcoz_Durations_Map_Mutex");
	scoped_lock<named_mutex> lock(_durationsMutex);

	managed_shared_memory segment(open_only, "gcoz_SharedMemory");
	IPC::DurationVector_Map* _baselineDurations = segment.find<IPC::DurationVector_Map>("Durations_Map").first;
	
	//for(auto method_it = _baselineDurations->begin(); method_it < _baselineDurations->end(); method_it++)
	for (const auto& method : *_baselineDurations) 
	{// iterate through map<int, DurationVector>
		if (method.second.size() > 0) 
		{// Durations measured for method
			_idCollectorMethodVector.push(method.first);
			Nanoseconds average = Nanoseconds(0);
			for (const auto& dur : method.second) {
				average += dur;
			}
			average /= method.second.size();
			_baselineDurationsAverage.emplace(method.first, average);
			spdlog::warn("Size of _baselineDurationsAverage: {}", _baselineDurationsAverage.size());
			spdlog::info("Average for method {} is: {}", methodNames[method.first], average.count());
			if (average > Nanoseconds(1000))
			{// 100ns clock accuracy (1000 * 0.1 = 100)
				choice current;
				current.method = method.first;
				for (int i = 0; i < amoutSpeedupsMax; i++) {
					current.speedup = i * 10;
					choices.push_back(current);
				}
			}
		}
	}

	std::shuffle(choices.begin(), choices.end(), gen);
	spdlog::info("[DelayCalculator] created {} combinations of method/speedup", choices.size());
	printBaseline();
	baselineAdded = true;
}

void DelayCalculator::calculateDelays(int& _speedupPicked, int& _methodPicked, delayArray& _msgDelays) {

	choice newChoice = { 0, 0 };
	int selectedSpeedup;
	int selectedMethod;

	if (allMethodsDelayedDone && choices.size() > 0) {
		newChoice = choices.front();
		choices.pop_front();
	}
	else {
		spdlog::info("Delaying all methods");
	}

	static int allSpeedup = 0;
	selectedSpeedup = allMethodsDelayedDone ? newChoice.speedup : allSpeedup;
	selectedMethod = allMethodsDelayedDone ? newChoice.method : -1;
	allSpeedup += 10;

	spdlog::info("Selected: {} with speedup {}, ({} combinations remaining)", selectedMethod, selectedSpeedup, choices.size());

	/*  calculate delays for methods. This delay is at least 1ns
		first "if" sets delay for all methods except selectedMethod
		second "if" set delay for selectedMethod as well if frametimeChangesAll is not full yet
		third "if" sets delay to basically none, just in case a method that wasn't called during measuring is called during profiling
	*/
	for (int i = 0; i < D3D11_METHOD_COUNT; i++) 
	{
		Nanoseconds average = _baselineDurationsAverage[i];
		float delay = static_cast<float>(selectedSpeedup) / 100;
		if (average > Nanoseconds(0)) {
			//spdlog::warn("Delay for {}: {} (average: {})", i, std::llround(static_cast<float>(average) * delay), average);
		}
		if (_baselineDurationsAverage[i] > Nanoseconds(1000))
		{// 100ns == resolution of high_resolution_clock
			if (i != selectedMethod)
			{
				_msgDelays[i] = std::chrono::duration_cast<Nanoseconds>(average * delay);
			}
			else if (!allMethodsDelayedDone) {
				_msgDelays[i] = std::chrono::duration_cast<Nanoseconds>(average * delay);
			}
			else {
				_msgDelays[i] = Nanoseconds(0);
			}
		}
		else {
			_msgDelays[i] = Nanoseconds(0);
		}
	}

	_methodPicked = selectedMethod;
	_speedupPicked = selectedSpeedup;
}

void DelayCalculator::measurementDoneAll(int _speedup) {
	frametimeChangesAll[_speedup] = true;
	if (frametimeChangesAll.size() == amoutSpeedupsMax) {
		allMethodsDelayedDone = true;
	}
}

void DelayCalculator::measurementDoneSingle(int _speedup, int _method) {
	frametimeChangesSingle[_method][_speedup] = true;
}

bool DelayCalculator::dataCollectedAllMethods() {
	return allMethodsDelayedDone;
}

bool DelayCalculator::dataCollected() {
	return choices.empty();
}