#pragma once
#include <Windows.h>
#include <map>
#include <array>
#include <queue>
#include <deque>
#include <format>
#include <random>
#include <chrono>
#include <iostream>
#include <numeric>
#include <time.h>
#include <spdlog/spdlog.h>
#include <Constants.h>
#include <status.h>
#include <SharedMemoryBoost.hpp>

static std::random_device rd;
static std::mt19937 gen(rd());

/* A class that uses the measured duration of each method
*  and the currently picked 'speedup' percentage and 
*  method in order to calculate how long to delay each
*  method other to simulate the picked method being faster
*/
class DelayCalculator {

	struct choice {
		int method;
		int speedup;
	};

	// define it here just to have a name for it
	int amoutSpeedupsMax = 10; // 0.1 - 0.9 in .1 increments

	// Contains all combinations of methods/'speedups'
	std::deque<choice> choices;
	// filled with averages from addBaseline()
	std::map<int, Nanoseconds> _baselineDurationsAverage;

	bool baselineAdded;
	// all 10 different "speedups" applied to all methods at once
	bool allMethodsDelayedDone = false;
	// Tracks wether delay has been applied to all methods. If element at "speedup" exists, it has been done
	std::map<int, bool> frametimeChangesAll;

	// Tracks for all methods wether delay has been applied, works same as above
	std::array<std::map<int, bool>, D3D11_METHOD_COUNT> frametimeChangesSingle;

	// print measures averages in a more or less readable format
	void printBaseline();

public:
	DelayCalculator();
	bool isBaselineAdded() { return baselineAdded; };
	// returns true if no more combinations are in the queue
	bool dataCollected();
	bool dataCollectedAllMethods() { return allMethodsDelayedDone; };
	// average the measured duration for each method to calculate delays from
	void addBaseline(std::queue<int>& _idCollectorMethodVector);
	// use calculated averages to calculate delay for each method
	void calculateDelays(int& _speedupPicked, int& _methodPicked, delayArray& _msgDelays);
	// bit of logic to set 'progress booleans'
	void measurementDoneAll(int _speedup);
	void measurementDoneSingle(int _speedup, int _method);
};