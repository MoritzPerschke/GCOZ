#include "DelayCalculator.h"

int DelayCalculator::pickMethod() {
	int picked = method(gen);
	if (baselineDurations[picked].count() == 0) {
		picked = pickMethod();
	}
	return picked;
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


long long int averageFrametime(std::array<Nanoseconds, MEASURE_FRAME_COUNT> _frameTimes) {
	long long int average = 0;
	for (const auto& frameTime : _frameTimes) {
		average += frameTime.count() / _frameTimes.size();
	}
	return average;
}

void DelayCalculator::printBaseline() {
	std::cout <<
		"= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= =" << std::endl <<
		"Baseline Method Durations:" << std::endl;
	for (const auto& dur : baselineDurations) {
		std::cout << std::setfill(' ') << std::setw(8) << dur.count();
	}
	std::cout << std::endl << std::endl <<
		"Baseline frame time average: " << baselineAverageFrameTime << std::endl <<
		"= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= =" << std::endl;
}

void DelayCalculator::addBaseline(std::array<Nanoseconds, D3D11_METHOD_COUNT> _durations, std::array <Nanoseconds, MEASURE_FRAME_COUNT> _frameTimes) {
	baselineDurations = _durations; // straight copy
	baselineAverageFrameTime = averageFrametime(_frameTimes);

	printBaseline();
}

void DelayCalculator::calculateDelays(std::array<DWORD, D3D11_METHOD_COUNT>& _msgDelays) {
	int selectedMethod = pickMethod();
	float selectedSpeedup = pickSpeedup();

	std::cout << inf << "Selected Method " << selectedMethod << " with speedup " << selectedSpeedup << std::endl;

	if (isNewChoice(selectedMethod, selectedSpeedup)) {
		for (int i = 0; i < baselineDurations.size(); i++) {
			if (i != selectedMethod) {
				if (baselineDurations[i].count() != 0) {
					_msgDelays[i] = static_cast<DWORD>(1 + (baselineDurations[i].count() * selectedSpeedup)); // make sure this is not 0
					std::cout << std::setw(8) << std::setfill(' ') << i << ": " << _msgDelays[i];
				}
				else {
					_msgDelays[i] = 0; // just in case method wasn't called during measuring
				}
			}
		}
		std::cout << std::endl;
	}
	lastMethodProfiled = selectedMethod;
	lastSpeedup = selectedSpeedup;
}

void DelayCalculator::addResult(std::array<Nanoseconds, MEASURE_FRAME_COUNT> _frameTimes) {
	// calculate "speedup"
	uint64_t newAverage = averageFrametime(_frameTimes);
	uint64_t theoreticalAverage = static_cast<uint64_t> (baselineAverageFrameTime * (1 + lastSpeedup));
	float improvement = static_cast<float> (theoreticalAverage - newAverage) / theoreticalAverage;

	std::cout << ok << "Speedup " << lastSpeedup << " of Method " << lastMethodProfiled << " resulted in theoretical improvement of: " << improvement
		<< "\tBaseline: " << baselineAverageFrameTime << ", new: " << newAverage << ", theoretical: " << theoreticalAverage << std::endl;
	fpsImprovements[lastMethodProfiled][lastSpeedup] = improvement;

}

long long int DelayCalculator::getBaselineFt()
{
	return baselineAverageFrameTime;
}
