#pragma once
#include <Windows.h>
#include <fstream>
#include <array>
#include <vector>
#include <map>
#include "Constants.h"

class ResultsHandler {
	static std::vector<frametimeArray> allResults;
	static std::map<int, std::map<float, frametimeArray>> singleResults;

	void exportResultSingle();
	void exportResultAll();

public:
	void exportBaseline(frametimeArray _baselineTimes);
	void addResultSingle(frametimeArray _frameTimes, int _methodIndex, float _speedup);
	void addResultAll(frametimeArray _frameTimes, float _speedup);
	void exportResults();

};