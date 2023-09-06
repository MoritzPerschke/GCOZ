#pragma once
#include <chrono>
#include <array>
#include <vector>
#include <algorithm>
#include "ProfilerStatus.h"
#include "Constants.h"

using Timepoint = std::chrono::steady_clock::time_point;
using Duration = std::chrono::duration<double>;

struct Message {
	bool valid;
};

struct ProfilerMessage : Message {
	delayArray delays;
	ProfilerStatus status;
};

struct DllMessage : Message {
	ProfilerStatus lastStatus;
	frametimeArray frameTimes;
	frametimeArray frameRates;
	durationArray durations;
	std::array<int, D3D11_METHOD_COUNT> methodCalls;
};
