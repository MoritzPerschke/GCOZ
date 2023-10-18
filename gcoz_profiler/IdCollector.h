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
	int currentMethod;
	int lastMethod;
	std::map<idHash, std::vector<long long>> ids; // no idea how many different threads might call a method

public:
	IdCollector();
	void addIDs(idArray _threadIDs);
	int nextMethod();
	bool isDone();
	void finish(ResultsHandler& resH);
};