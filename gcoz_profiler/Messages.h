#pragma once
#include <chrono>
#include <array>
#include <vector>
#include "ProfilerStatus.h"
#include "Constants.h"

using Timepoint = std::chrono::steady_clock::time_point;
using Duration = std::chrono::duration<double>;
using Nanoseconds = std::chrono::nanoseconds;

struct Message {
	bool valid;
};

struct ProfilerMessage : Message {
	std::array<DWORD, D3D11_METHOD_COUNT> delays;
	ProfilerStatus status;
};

struct DllMessage : Message {
	ProfilerStatus lastStatus;
	std::vector<Duration> frameTimepoints;
	std::array<Nanoseconds, D3D11_METHOD_COUNT> durations;
};
