#pragma once

#include <Windows.h>
#include "ProfilerStatus.h"
#include "ResultsHandler.h"
#include "DelayCalculator.h"
#include "Messages.h"
#include "IdCollector.h"

class ProfilerStatusManager {
	float lastSpeedup;
	int lastMethodIndex;

	ResultsHandler resultsHandler;
	DelayCalculator calc;
	IdCollector ids;

	void nextMessage(ProfilerMessage& _msg);

public:
	ProfilerStatusManager(string& _processName);
	ProfilerStatus next(DllMessage _dllMsg, ProfilerMessage& _nextMessage);
	void finish();
	bool dataCollected() { return calc.dataCollected(); }
};