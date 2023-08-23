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
	void finalMessage(ProfilerMessage& _msg);

public:
	ProfilerStatusManager(string& _processName);
	ProfilerStatus next(DllMessage _dllMsg, ProfilerMessage& _nextMessage);
	void finish();
	bool dataCollected() { return calc.dataCollected(); }
};