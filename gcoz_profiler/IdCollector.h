#pragma once
#include <Windows.h>
#include <map>
#include <vector>
#include <array>
#include <thread>
#include <iostream>
#include <spdlog/spdlog.h>

#include "ResultsHandler.h"
#include "../shared/status.h"
#include "../shared/Constants.h"

class IdCollector {
	int currentMethod;
	int lastMethod;
	std::map<int, std::vector<idHash>> ids; // no idea how many different threads might call a method

public:
	IdCollector();
	void addIDs(idArray _threadIDs);
	int nextMethod();
	bool isDone();
	void finish(ResultsHandler& resH);
};