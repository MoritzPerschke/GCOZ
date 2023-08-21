#pragma once
#include <Windows.h>
#include <map>
#include <array>
#include <format>
#include <random>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include "Constants.h"
#include "status.h"

using Duration = std::chrono::duration<double>;
using Nanoseconds = std::chrono::nanoseconds;

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<int> method(0, D3D11_METHOD_COUNT);
static std::uniform_int_distribution<int> speedup(0, 9);

class DelayCalculator { // maybe this can be changed to be specific to one speedup/method combo or sth, make it more readable
	// ugly, but: a map where all the frametime improvement per speedup
	// percentage is tracked
	// e.g. 10% increase in present results in 5% speedup in frametime -> 
	// improvements[8].at(0.1) = 0.05
	std::array<std::map<float, long long>, D3D11_METHOD_COUNT> frametimeChangesSingle; // tracking this could be moved to seperate class
	std::map<float, long long> frametimeChangesAll;
	int amoutSpeedupsMax = 10; // 0.1 - 0.9 in .1 increments
	bool allMethodSpeedupsDone = false;

	long long baselineAverageFrameTime;
	durationArray baselineDurations;

	int lastMethodProfiled;
	float lastSpeedup;
	delayArray lastDelaysApplied;

	int pickMethod();
	float pickSpeedup();
	bool isNewChoice(int _method, float _speedup);
	void printBaseline();

public:
	void addBaseline(durationArray _durations, frametimeArray _frameTimes);
	void calculateDelays(delayArray& _msgDelays);
	void addResult(frametimeArray _frameTimes);
	void calculateResults();
	bool allDataCollected();
};
