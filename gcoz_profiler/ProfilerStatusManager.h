#pragma once

#include <Windows.h>
#include "ProfilerStatus.h"
#include "DelayCalculator.h"
#include "Messages.h"

class ProfilerStatusManager {
	DelayCalculator calc;

	ProfilerStatus next(DllMessage _dllMsg, ProfilerMessage& _nextMessage);
};