#pragma once
#include <Windows.h>
#include <map>
#include <queue>
#include <array>
#include <thread>
#include <iostream>
#include <spdlog/spdlog.h>

#include <status.h>
#include <Constants.h>

/* Class providing functionality to collect information
*  about which thread calls which methods
*/
class IdCollector {
	std::queue<int> calledMethods;
	int currentMethod;

public:
	IdCollector();
	void idsAdded();
	int nextMethod();
	bool isDone();
	std::queue<int>& getRefMethodQueue();
};