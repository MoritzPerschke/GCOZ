#pragma once
#include <Windows.h>
#include <map>
#include <array>
#include <format>
#include <random>
#include <chrono>
#include <iostream>
#include <iomanip>
#include "Constants.h"
#include "status.h"

using Duration = std::chrono::duration<double>;
using Nanoseconds = std::chrono::nanoseconds;

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<int> method(0, D3D11_METHOD_COUNT);
static std::uniform_int_distribution<int> speedup(0, 9);

class DelayCalculator {
	// ugly, but: a map where all the frametime improvement per speedup
	// percentage is tracked
	// e.g. 10% increase in present results in 5% speedup in frametime -> 
	// improvements[8].at(0.1) = 0.05
	std::array<std::map<float, Duration>, D3D11_METHOD_COUNT> fpsImprovements; // tracking this could be moved to seperate class

	long long int baselineAverageFrameTime;
	std::array<Nanoseconds, D3D11_METHOD_COUNT> baselineDurations;

	int lastMethodProfiled;
	float lastMethodSpeedup;
	std::array<Nanoseconds, D3D11_METHOD_COUNT> lastDelaysApplied;

	int pickMethod();
	float pickSpeedup();
	bool isNewChoice(int _method, float _speedup);
	void printBaseline();

public:
	void addBaseline(std::array<Nanoseconds, D3D11_METHOD_COUNT> _durations, std::array <Nanoseconds, MEASURE_FRAME_COUNT> _frameTimes);
	void calculateDelays(std::array<DWORD, D3D11_METHOD_COUNT>& _msgDelays);
	void addResult(std::array <Nanoseconds, MEASURE_FRAME_COUNT> _frameTimes);
	long long int getBaselineFt();
};
