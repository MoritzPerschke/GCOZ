#pragma once
#include <Windows.h>
#include <map>
#include <vector>
#include <array>
#include <thread>
#include <iostream>
#include <spdlog/spdlog.h>

#include <status.h>
#include <Constants.h>

class IdCollector {
	int currentMethod;

public:
	IdCollector();
	void idsAdded();
	int nextMethod();
	bool isDone();
};