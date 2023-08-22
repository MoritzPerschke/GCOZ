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

namespace fs = std::filesystem;
using json = nlohmann::json;
using std::copy;
using std::to_string;
using std::string;

class ResultsHandler {
	fs::path filePath;
	json outputJson;
	/*void exportResultSingle(frametimeArray _resultingTimes, int _methodIndex, float _speedup);
	void exportResultAll(frametimeArray _resultingTimes, float _speedup);*/

public:
	ResultsHandler();
	ResultsHandler(string& _processName);
	void addBaseline(frametimeArray _baselineTimes);
	void addResultSingle(frametimeArray _frameTimes, int _methodIndex, float _speedup);
	void addResultAll(frametimeArray _frameTimes, float _speedup);
	void exportResults();
};