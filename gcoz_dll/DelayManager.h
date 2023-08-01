#pragma once
#include <Windows.h>
#include <chrono>
#include <array>
#include "Communication.h"

class DelayManager {
	std::array<DWORD, 205> methodDelays;
public:
	DelayManager();
	DWORD getDelay(int methodIndex);
	void updateDelays(Communication com);
};