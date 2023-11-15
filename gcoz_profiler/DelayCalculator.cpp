#include "DelayCalculator.h"

DelayCalculator::DelayCalculator() {
}

void DelayCalculator::printBaseline() { // this probably won't be needed anymore
	std::cout <<
		"= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= =" << std::endl <<
		"Baseline Method Durations:" << std::endl;
	for (int i = 0; i < _baselineDurationsAverage.size(); i++) {
		std::cout << std::setfill(' ') << std::setw(7) << _baselineDurationsAverage[i];
		if ((i + 1) % 19 == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << std::endl <<
		"= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= =" << std::endl;
}

void printDelays(delayArray& _delays) { // this probably won't be needed anymore
	std::cout <<
		"= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= =" << std::endl <<
		"Baseline Method Durations:" << std::endl;
	for (int i = 0; i < _delays.size(); i++) {
		std::cout << std::setfill(' ') << std::setw(7) << _delays[i].count() << "ns";
		if ((i + 1) % 19 == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << std::endl <<
		"= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= =" << std::endl;
}

void DelayCalculator::addBaseline() {
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
			SharedDuration average = 0;
			for (const auto& dur : method.second) {
				average += dur;
			}
			average /= method.second.size();
			_baselineDurationsAverage.push_back(average);
			spdlog::info("Average for method {} is: {}", methodNames[method.first], average);
			if (average < 1000)
			{// 1000ns clock accuracy
				choice current;
				current.method = method.first;
				for (int i = 0; i < amoutSpeedupsMax; i++) {
					current.speedup = static_cast<float>(i) / 10;
					choices.push_back(current); /// TODO: add back
				}
			}
		}
	}

	std::shuffle(choices.begin(), choices.end(), gen);
	spdlog::info("[DelayCalculator] created {} combinations of method/speedup", choices.size());
	//printBaseline();
	baselineAdded = true;
}

void DelayCalculator::calculateDelays(float& _speedupPicked, int& _methodPicked, delayArray& _msgDelays) {

	choice newChoice = { 0, .0 };
	float selectedSpeedup;
	int selectedMethod;

	if (allMethodsDelayedDone && choices.size() > 0) {
		newChoice = choices.front();
		choices.pop_front();
	}
	else {
		spdlog::info("Delaying all methods");
	}

	static float allSpeedup = static_cast<float>(0.0);
	selectedSpeedup = allMethodsDelayedDone ? newChoice.speedup : allSpeedup;
	selectedMethod = allMethodsDelayedDone ? newChoice.method : -1;
	allSpeedup += static_cast<float>(0.1);

	spdlog::info("Selected: {} with speedup {}, {} combinations remaining", selectedMethod, selectedSpeedup, choices.size());

	/*  calculate delays for methods. This delay is at least 1ns
		first "if" sets delay for all methods except selectedMethod
		second "if" set delay for selectedMethod as well if frametimeChangesAll is not full yet
		third "if" sets delay to basically none, just in case a method that wasn't called during measuring is called during profiling
	*/
	for (int i = 0; i < _baselineDurationsAverage.size(); i++) 
	{
		SharedDuration average = _baselineDurationsAverage[i];
		if (_baselineDurationsAverage[i] > 1000)
		{// 100ns == resolution of high_resolution_clock
			if (i != selectedMethod)
			{
				_msgDelays[i] = Nanoseconds(std::llround(average * selectedSpeedup));
			}
			else if (!allMethodsDelayedDone) {
				_msgDelays[i] = Nanoseconds(std::llround(average * selectedSpeedup));
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

void DelayCalculator::measurementDoneAll(float _speedup) {
	frametimeChangesAll[_speedup] = true;
	if (frametimeChangesAll.size() == amoutSpeedupsMax) {
		allMethodsDelayedDone = true;
	}
}

void DelayCalculator::measurementDoneSingle(float _speedup, int _method) {
	frametimeChangesSingle[_method][_speedup] = true;
}

bool DelayCalculator::dataCollectedAllMethods() {
	return allMethodsDelayedDone;
}

bool DelayCalculator::dataCollected() {
	return choices.empty();
}