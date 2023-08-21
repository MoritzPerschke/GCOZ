#pragma once

#include <Windows.h>
#include "ProfilerStatus.h"
#include "ResultsHandler.h"
#include "DelayCalculator.h"
#include "Messages.h"

class ProfilerStatusManager {
	float lastSpeedup;
	int lastMethodIndex;

	DelayCalculator calc;
	ResultsHandler resultsHandler;

	void newMessage(ProfilerMessage& _msg);

public:
	ProfilerStatus next(DllMessage _dllMsg, ProfilerMessage& _nextMessage);
	bool dataCollected() { return calc.dataCollected(); }
	void getResults() { calc.calculateResults(); }
};