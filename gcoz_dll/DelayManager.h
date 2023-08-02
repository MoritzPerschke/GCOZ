#pragma once
#include <Windows.h>
#include <chrono>
#include <array>
#include "Communication.h"

// maybe this can be a namespace
class DelayManager {
	std::array<DWORD, 205> methodDelays;
public:
	DelayManager();
	DWORD getDelay(int methodIndex);
	void updateDelays(std::array<DWORD, 205> _newDelays);
};