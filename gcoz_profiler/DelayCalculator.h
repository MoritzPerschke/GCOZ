#pragma once
#include <Windows.h>
#include <map>
#include <array>
#include <deque>
#include <format>
#include <random>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <time.h>
#include "Constants.h"
#include "status.h"

using Duration = std::chrono::duration<double>;

static std::random_device rd;
static std::mt19937 gen(rd());
//static std::uniform_int_distribution<int> method(0, (D3D11_METHOD_COUNT-1));
//static std::uniform_int_distribution<int> speedup(0, 9);

class DelayCalculator {

	struct choice {
		int method;
		float speedup;
	};
	
	std::deque<choice> choices;
	int amoutSpeedupsMax = 10; // 0.1 - 0.9 in .1 increments
	bool allMethodSpeedupsDone = false; // all 10 different "speedups" applied to all methods at once
	std::array<int, D3D11_METHOD_COUNT> baselineCalls;
	durationArray baselineDurations; // these are used to calculate delays
	int lastMethodProfiled; // when a new msg is received from dll, this method was profiled
	float lastSpeedup; // same as above for speedup

	/* Tracks wether "speedup" float has been applied to all methods. If element at "speedup" exists, it has been done*/
	std::map<float, bool> frametimeChangesAll;
	/* Tracks for all methods wether "speedup" float has been applied, works same as above*/
	std::array<std::map<float, bool>, D3D11_METHOD_COUNT> frametimeChangesSingle;

	void printBaseline();

public:
	bool dataCollectedAllMethods();
	bool dataCollected();
	void addBaseline(durationArray _durations, frametimeArray _frameTimes, std::array<int, D3D11_METHOD_COUNT> _calls);
	void calculateDelays(float& _speedupPicked, int& _methodPicked, delayArray& _msgDelays);
	void measurementDone();
};
