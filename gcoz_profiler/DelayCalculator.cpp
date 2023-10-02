#include "DelayCalculator.h"

std::string getTime() {
	std::tm bt {};
	auto timer = std::time(0);
	localtime_s(&bt, &timer);
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%R", &bt);
	return "["+ std::string(buffer) + "]";
}

DelayCalculator::DelayCalculator() {
}

DelayCalculator::DelayCalculator(ResultsHandler& _handler){
	handler = _handler;
}

bool DelayCalculator::dataCollectedAllMethods() { return allMethodSpeedupsDone; }

void DelayCalculator::printBaseline() { // this probably won't be needed anymore
	std::cout <<
		"= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= =" << std::endl <<
		"Baseline Method Durations:" << std::endl;
	for (int i = 0; i < baselineDurations.size(); i++) {
		std::cout << std::setfill(' ') << std::setw(7) << baselineDurations[i].count();
		if ((i+1) % 19 == 0) {
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

void DelayCalculator::addBaseline(durationArray _durations, frametimeArray _frameTimes, std::array<int, D3D11_METHOD_COUNT> _calls) {
	baselineDurations = _durations;
	baselineCalls = _calls;
	handler.addBaseline(_frameTimes, _durations, _calls);
	for (int i = 0; i < D3D11_METHOD_COUNT; i++) {
		if (baselineCalls[i] > 0) {
			baselineDurations[i] = baselineDurations[i] / baselineCalls[i];

			if (baselineDurations[i] > Nanoseconds(1000)) {
				choice current;
				current.method = i;
				for (int j = 0; j < amoutSpeedupsMax; j++) {
					current.speedup = static_cast<float>(j) / 10;
					choices.push_back(current);
				}
			}
		}
	}
	std::shuffle(choices.begin(), choices.end(), gen);
	std::cout << ok << getTime() << "[DelayCalculator] created " << choices.size() << "combinations of method/speedup" << std::endl;
	printBaseline();

}

void DelayCalculator::calculateDelays(float& _speedupPicked, int& _methodPicked, delayArray& _msgDelays) {
	static float allSpeedup = static_cast<float>(-0.1);
	allSpeedup += static_cast<float>(0.1);

	choice newChoice = { 0, .0 };
	float selectedSpeedup;
	int selectedMethod;

	if (allMethodSpeedupsDone && choices.size() > 0) {
		while (baselineDurations[choices.front().method] < Nanoseconds(1000)) {
			choices.pop_front();
		}
		newChoice = choices.front();
		choices.pop_front();
	}
	
	selectedSpeedup= allMethodSpeedupsDone ? newChoice.speedup : allSpeedup;
	selectedMethod = allMethodSpeedupsDone ? newChoice.method : -1;

	std::cout << "\n" << inf << getTime() << "[DelayCalculator] Selected: " << selectedMethod << " with speedup " << selectedSpeedup << ", " <<  choices.size() << " combinations remaining" << std::endl;

	/*  calculate delays for methods. This delay is at least 1ns
		first "if" sets delay for all methods except selectedMethod
		second "if" set delay for selectedMethod as well if frametimeChangesAll is not full yet
		third "if" sets delay to basically none, just in case a method that wasn't called during measuring is called during profiling
	*/
	for (int i = 0; i < baselineDurations.size(); i++) {
		if (baselineDurations[i] > Nanoseconds(1000)) { // 100ns == resolution of high_resolution_clock
			if (i != selectedMethod) {
				_msgDelays[i] = std::chrono::duration_cast<Nanoseconds>(baselineDurations[i] * selectedSpeedup);
			}
			else if (!allMethodSpeedupsDone) {
				_msgDelays[i] = std::chrono::duration_cast<Nanoseconds>(baselineDurations[i] * selectedSpeedup);
			}
			else {
				_msgDelays[i] = Nanoseconds(0);
			}
		}
		else {
			_msgDelays[i] = Nanoseconds(0);
		}
	}

	_methodPicked = lastMethodProfiled = selectedMethod;
	_speedupPicked = lastSpeedup = selectedSpeedup;
}

void DelayCalculator::measurementDone() {

	if (!allMethodSpeedupsDone) {
		frametimeChangesAll[lastSpeedup] = true;
		if (frametimeChangesAll.size() == amoutSpeedupsMax) {
			allMethodSpeedupsDone = true;
		}
	}
	else {
		frametimeChangesSingle[lastMethodProfiled][lastSpeedup] = true;
	}

}

bool DelayCalculator::dataCollected() {
	return choices.empty();
}