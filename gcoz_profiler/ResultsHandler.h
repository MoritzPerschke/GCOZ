#pragma once
#include <Windows.h>
#include <fstream>
#include <array>
#include <vector>
#include <map>
#include <filesystem>
#include <Psapi.h>
#include <nlohmann/json.hpp>
#include "Constants.h"
#include "status.h"

#include <iostream>

const static std::array<std::string, D3D11_METHOD_COUNT> methodNames = {
#define X(_MethodName)\
	#_MethodName,
	D3D11_METHOD_NAMES
#undef X
};

namespace fs = std::filesystem;
using json = nlohmann::json;
using std::copy;
using std::to_string;
using std::string;

class ResultsHandler {
	fs::path filePathTimes;
	fs::path filePathRates;
	fs::path filePathBaseline;

	json outputJsonRates;
	json outputJsonTimes;
	json outputJsonBaseline;
	/*void exportResultSingle(frametimeArray _resultingTimes, int _methodIndex, float _speedup);
	void exportResultAll(frametimeArray _resultingTimes, float _speedup);*/

public:
	ResultsHandler();
	ResultsHandler(string& _processName);
	void addBaseline(frametimeArray _baselineTimes, durationArray _baselineDurations, std::array<int, D3D11_METHOD_COUNT> _calls);
	void addResultSingle(frametimeArray _frameTimes, frametimeArray _frameRates, int _methodIndex, float _speedup);
	void addResultAll(frametimeArray _frameTimes, frametimeArray _frameRates, float _speedup);
	void exportResults();
};