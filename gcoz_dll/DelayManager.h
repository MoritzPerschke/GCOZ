#pragma once
#include <Windows.h>
#include <chrono>
#include <array>
#include "Communication.h"
#include "../gcoz_profiler/Constants.h"

// maybe this can be a namespace
class DelayManager {
	delayArray methodDelays;
public:
	DelayManager();
	Nanoseconds getDelay(int methodIndex);
	void updateDelays(delayArray _newDelays);
	void resetDelays();
};