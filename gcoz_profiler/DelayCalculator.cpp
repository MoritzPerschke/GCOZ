#include "DelayCalculator.h"

int DelayCalculator::pickMethod() {
	return method(gen);
}

float DelayCalculator::pickSpeedup() {
	return (float)speedup(gen) / 10;
}

bool DelayCalculator::isNewChoice(int _method, float _speedup) {
	if (fpsImprovements[_method].find(_speedup) == fpsImprovements[_method].end()){
		return true;
	}
	return false;
}

void DelayCalculator::printBaseline() {
	std::cout <<
		"= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= =" << std::endl <<
		"Baseline Method Durations:" << std::endl;
	for (const auto& dur : baselineDurations) {
		std::cout << std::setfill(' ') << std::setw(7) << dur.count() << '\t';
	}
	std::cout << std::endl <<
		"Baseline frame time average: " << baselineAverageFrameTime << std::endl <<
		"= ======= ======= ======= ======= ======= ======= ======= ======= ======= =" << std::endl;
}

void DelayCalculator::addBaseline(std::array<Nanoseconds, D3D11_METHOD_COUNT> _durations, std::array <Nanoseconds, MEASURE_FRAME_COUNT> _frameTimes) {
	baselineDurations = _durations; // straight copy

	baselineAverageFrameTime = 0; // reset to 0 and recalculate
	for (const auto& frameTime : _frameTimes) {
		baselineAverageFrameTime += frameTime.count() / _frameTimes.size();
	}
	printBaseline();
}

void DelayCalculator::calculateDelays(std::array<DWORD, D3D11_METHOD_COUNT>& _msgDelays) {
	int selectedMethod = pickMethod();
	float selectedSpeedup = pickSpeedup();

	std::cout << inf << "Selected Method " << selectedMethod << " with speedup " << selectedSpeedup << std::endl;

	if (isNewChoice(selectedMethod, selectedSpeedup)) {
		for (int i = 0; i < baselineDurations.size(); i++) {
			if (i != selectedMethod) {
				_msgDelays[i]  = baselineDurations[i].count() * selectedSpeedup;
			}
		}
	}

	lastMethodProfiled = selectedMethod;
	lastMethodSpeedup = selectedSpeedup;

}

void DelayCalculator::addResult(std::array<Nanoseconds, MEASURE_FRAME_COUNT> _frameTimes) {
	// calculate "speedup"
	//fpsImprovements[lastMethodProfiled].at(lastMethodSpeedup) = "speedup";
}