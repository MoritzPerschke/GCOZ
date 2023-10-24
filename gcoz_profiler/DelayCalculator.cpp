#include "DelayCalculator.h"

void DelayCalculator::printBaseline() { // this probably won't be needed anymore
	std::cout <<
		"= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= =" << std::endl <<
		"Baseline Method Durations:" << std::endl;
	for (int i = 0; i < baselineDurations.size(); i++) {
		std::cout << std::setfill(' ') << std::setw(7) << baselineDurations[i].count();
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

void DelayCalculator::addBaseline(frametimeArray _frameTimes, durationArray _durations, std::array<int, D3D11_METHOD_COUNT> _calls) {
	baselineDurations = _durations;
	baselineCalls = _calls;

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
	spdlog::info("[DelayCalculator] created {} combinations of method/speedup", choices.size());
	//printBaseline();
	baselineAdded = true;

}

void DelayCalculator::calculateDelays(float& _speedupPicked, int& _methodPicked, delayArray& _msgDelays) {

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
	else {
		spdlog::info("Delaying all methods");
	}

	static float allSpeedup = static_cast<float>(0.0);
	selectedSpeedup = allMethodSpeedupsDone ? newChoice.speedup : allSpeedup;
	selectedMethod = allMethodSpeedupsDone ? newChoice.method : -1;
	allSpeedup += static_cast<float>(0.1);

	spdlog::info("Selected: {} with speedup {}, {} combinations remaining", selectedMethod, selectedSpeedup, choices.size());

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

	_methodPicked = selectedMethod;
	_speedupPicked = selectedSpeedup;
}

void DelayCalculator::measurementDoneAll(float _speedup) {
	frametimeChangesAll[_speedup] = true;
	if (frametimeChangesAll.size() == amoutSpeedupsMax) {
		allMethodSpeedupsDone = true;
	}
}

void DelayCalculator::measurementDoneSingle(float _speedup, int _method) {
	frametimeChangesSingle[_method][_speedup] = true;
}

bool DelayCalculator::dataCollectedAllMethods() {
	return allMethodSpeedupsDone;
}

bool DelayCalculator::dataCollected() {
	return choices.empty();
}