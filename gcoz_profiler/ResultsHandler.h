#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <map>
#include <filesystem>
#include <Psapi.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <SharedMemoryBoost.hpp>
#include <Constants.h>
#include <status.h>

namespace fs = std::filesystem;
using json = nlohmann::json;
using std::copy;
using std::to_string;
using std::string;

class ResultsHandler {
	std::string _outputName;

public:
	json getMethodDurations();
	json getFrameRates();
	json getFrameTimes();
	json getThreadIDs();
	void exportResults(string _outputName);
};