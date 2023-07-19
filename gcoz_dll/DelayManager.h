#pragma once
#include <Windows.h>
#include <chrono>
#include <array>
#include "Communication.h"

class DelayManager {
	std::array<DWORD, 205> methodDelays;
	Communication com;
public:
	DelayManager();
	DelayManager(Communication _com);
	DWORD getDelay(int methodIndex);
	void updateDelays();
};