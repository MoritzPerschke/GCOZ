#pragma once
#include <chrono>
#include <array>
#include <vector>
#include <algorithm>
#include "ProfilerStatus.h"
#include "Constants.h"

struct Message {
	bool valid;
};

// this should be split into methodid collection and profiling
struct ProfilerMessage : Message {
	delayArray delays;
	int nextMethod;
};

struct DllMessage : Message {};

struct Measurement : DllMessage {
	frametimeArray frameTimes;
	durationArray durations;
	std::array<int, D3D11_METHOD_COUNT> methodCalls;
};

struct Result : DllMessage {
	frametimeArray frameTimes;
	frametimeArray frameRates;
};

struct ThreadIDMessage : DllMessage {
	idArray threadIDs;
};