#pragma once
#include <Windows.h>
#include <chrono>
#include <array>
#include "Communication.h"
#include "../gcoz_profiler/Constants.h"

// maybe this can be a namespace
class DelayManager {
	std::array<DWORD, D3D11_METHOD_COUNT> methodDelays;
public:
	DelayManager();
	DWORD getDelay(int methodIndex);
	void updateDelays(std::array<DWORD, D3D11_METHOD_COUNT> _newDelays);
	void resetDelays();
};