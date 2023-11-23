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

class DelayCalculator {

	struct choice {
		int method;
		int speedup;
	};

	int amoutSpeedupsMax = 10; // 0.1 - 0.9 in .1 increments

	std::deque<choice> choices;
	std::vector<SharedDuration> _baselineDurationsAverage; // filled with averages from addBaseline()

	bool baselineAdded;
	bool allMethodsDelayedDone = false; // all 10 different "speedups" applied to all methods at once

	/* Tracks wether delay has been applied to all methods. If element at "speedup" exists, it has been done*/
	std::map<int, bool> frametimeChangesAll;

	/* Tracks for all methods wether delay has been applied, works same as above*/
	std::array<std::map<int, bool>, D3D11_METHOD_COUNT> frametimeChangesSingle;

	void printBaseline();

public:
	DelayCalculator();
	bool isBaselineAdded() { return baselineAdded; };
	bool dataCollected();
	bool dataCollectedAllMethods();
	void addBaseline(std::queue<int>& _idCollectorMethodVector);
	//void addBaseline(frametimeArray _frameTimes, durationArray _durations, std::array<int, D3D11_METHOD_COUNT> _calls);
	void calculateDelays(int& _speedupPicked, int& _methodPicked, delayArray& _msgDelays);
	void measurementDoneAll(int _speedup);
	void measurementDoneSingle(int _speedup, int _method);
};