#include "DelayCalculator.h"

bool DelayCalculator::dataCollectedAllMethods() { return allMethodSpeedupsDone; }

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
	if (!allMethodSpeedupsDone) {
		return frametimeChangesAll.find(_speedup) == frametimeChangesAll.end();
	}
	else {
		return (frametimeChangesSingle[_method].find(_speedup) == frametimeChangesSingle[_method].end());
	}
	return false;
}


long long int averageFrametime(frametimeArray _frameTimes) {
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
	for (int i = 0; i < baselineDurations.size(); i++) {
		std::cout << std::setfill(' ') << std::setw(7) << baselineDurations[i].count();
		if ((i+1) % 19 == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << std::endl <<
		"Baseline frame time average: " << baselineAverageFrameTime << std::endl <<
		"= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= =" << std::endl;
}

void DelayCalculator::addBaseline(durationArray _durations, frametimeArray _frameTimes) {
	baselineDurations = _durations; // straight copy
	baselineAverageFrameTime = averageFrametime(_frameTimes);

	printBaseline();
}

void DelayCalculator::calculateDelays(float& _speedupPicked, int& _methodPicked, delayArray& _msgDelays) {
	float selectedSpeedup;
	int selectedMethod;

	std::cout << "\nPicking combination speedup/method ";
	do{
		selectedSpeedup = pickSpeedup();
		selectedMethod = 8; /// TODO: revert
		std::cout << ".";
	}while (!isNewChoice(selectedMethod, selectedSpeedup));
	std::cout << inf << "\nSelected Method " << selectedMethod << " with speedup " << selectedSpeedup << std::endl;


	/*  calculate delays for methods. This delay is at least 1ms
		first "if" sets delay for all methods except selectedMethod
		second "if" set delay for selectedMethod as well if frametimeChangesAll is not full yet
		third "if" sets delay to basically none, just in case a method that wasn't called during measuring is called during profiling
	*/
	for (int i = 0; i < baselineDurations.size(); i++) {
		if (i != selectedMethod) {
			_msgDelays[i] = static_cast<DWORD>(1 + (baselineDurations[i].count() * selectedSpeedup));
		}
		else if(!allMethodSpeedupsDone){
			_msgDelays[i] = static_cast<DWORD>(1 + (baselineDurations[i].count() * selectedSpeedup));
			std::cout << "not done for all" << std::endl;
		}
		else {
			_msgDelays[i] = 1;
		}
	}

	_methodPicked = lastMethodProfiled = selectedMethod;
	_speedupPicked = lastSpeedup = selectedSpeedup;
}

void DelayCalculator::addResult(frametimeArray _frameTimes) { /// TODO move this

	long long newAverage = averageFrametime(_frameTimes);
	std::cout << "new Average: " << newAverage << std::endl;
	if (!allMethodSpeedupsDone) {
		frametimeChangesAll[lastSpeedup] = newAverage - baselineAverageFrameTime;
		if (frametimeChangesAll.size() == amoutSpeedupsMax) {
			allMethodSpeedupsDone = true;
			std::cout << ok << "All Frametimes for delays applied to all methods collected" << std::endl;
		}
	}
	else {
		frametimeChangesSingle[lastMethodProfiled][lastSpeedup] = newAverage - baselineAverageFrameTime;
		std::cout << "added frametime single at " << lastMethodProfiled << " " << lastSpeedup << std::endl;
	}

}

void DelayCalculator::calculateResults() { /// TODO: implement
	std::cout << ok << "Frametime differences for Delays on all methods:" << std::endl;
	for (const auto& diff : frametimeChangesAll) {
		std::cout << std::setw(3) << std::setfill(' ') << diff.first << ": " << std::setw(5) << diff.second;
	}
	std::cout << "\n\n" << ok << "Frametime differences for Present:" << "\n" << std::endl;
	for (const auto& diff : frametimeChangesSingle[8]) {
		std::cout << std::setw(3) << std::setfill(' ') << diff.first << ": " << std::setw(5) << diff.second;
	}
	std::cout << std::endl << "\"Improvement\" for Present:" << std::endl;
	std::cout << "\t Single/All" << std::endl;

	for (const auto& diff : frametimeChangesAll) {
		std::cout << std::setw(3) << diff.first << ": " << std::setw(8) << frametimeChangesSingle[8][diff.first] << " / " << diff.second << " = "
			<< static_cast<double>(frametimeChangesSingle[8][diff.first]) / static_cast<double>(diff.second) << std::endl;
	}
}

bool DelayCalculator::dataCollected() { /// TODO: implement

	size_t size = frametimeChangesAll.size();
	std::cout << "All: " << std::endl;
	for (const auto& res : frametimeChangesAll) {
		std::cout << res.first << ": " << res.second << ' ';
	}
	std::cout << "Size: " << size << "\nSingle:\n";
	size = frametimeChangesSingle[8].size();
	for (const auto& res : frametimeChangesSingle[8]) {
		std::cout << res.first << ": " << res.second << ' ';
	}
	std::cout << "Size: " << size << "\n\n";
	return frametimeChangesSingle[8].size() == amoutSpeedupsMax;
}