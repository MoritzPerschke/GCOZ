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
