#pragma once
#include <Windows.h>
#include <chrono>
#include <array>
#include <Constants.h>
#include <SharedMemoryBoost.hpp>
#include "Communication.h"

// maybe this can be a namespace
class DelayManager {
	delayArray methodDelays;

public:
	DelayManager();
	Nanoseconds getDelay(int methodIndex);
	void updateDelays(delayArray _newDelays);
	void resetDelays();
};