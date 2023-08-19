#pragma once

#include <Windows.h>
#include "ProfilerStatus.h"
#include "DelayCalculator.h"
#include "Messages.h"

class ProfilerStatusManager {
	DelayCalculator calc;

	void newMessage(ProfilerMessage& _msg);

public:
	ProfilerStatus next(DllMessage _dllMsg, ProfilerMessage& _nextMessage);
	bool allDataCollected() { return calc.allDataCollected(); }
	void getResults() { calc.calculateResults(); }
};