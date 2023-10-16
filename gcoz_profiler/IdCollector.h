#pragma once
#include <Windows.h>
#include <map>
#include <vector>
#include <array>
#include <thread>
#include <iostream>

#include "status.h"
#include "Constants.h"
#include "ResultsHandler.h"

class IdCollector {
	int lastMethod;
	std::map<int, std::vector<long long>> ids; // no idea how many different threads might call a method

public:
	IdCollector();
	void addIDs(std::array<long long, METHOD_ID_ARRAY_SIZE> _threadIDs);
	int nextMethod();
	bool isDone();
	void finish(ResultsHandler& resH);
};