#pragma once
#include <chrono>
#include <array>
#include <vector>
#include "ProfilerStatus.h"

using Timepoint = std::chrono::steady_clock::time_point;
using Duration = std::chrono::duration<double>;

struct Message {
	bool valid;
};

struct ProfilerMessage : Message {
	std::array<DWORD, 205> delays;
	ProfilerStatus status;
};

struct DllMessage : Message {
	ProfilerStatus lastStatus;
	std::vector<Duration> frameTimepoints;
	std::array<std::chrono::nanoseconds, 205> durations;
};
