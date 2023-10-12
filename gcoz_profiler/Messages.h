#pragma once
#include <chrono>
#include <array>
#include <vector>
#include <algorithm>
#include "ProfilerStatus.h"
#include "Constants.h"

using Timepoint = std::chrono::steady_clock::time_point;
using Duration = std::chrono::duration<double>; // should be removable

struct Message {
	bool valid;
};

// this should be split into methodid collection and profiling
struct ProfilerMessage : Message {
	ProfilerStatus status;
	// for profiling
	delayArray delays;
};

struct DllMessage : Message {};

struct Measurement : DllMessage {
	frametimeArray frameTimes;
	durationArray durations;
	std::array<int, D3D11_METHOD_COUNT> methodCalls;
	// for threadid collection
	std::array<long long, METHOD_ID_ARRAY_SIZE> threadIDs;
};

struct Result : DllMessage {
	frametimeArray frameTimes;
	frametimeArray frameRates;
};