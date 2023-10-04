#pragma once

#include <Windows.h>
#include <shared_mutex>
#include "ProfilerStatus.h"
#include "ResultsHandler.h"
#include "DelayCalculator.h"
#include "Messages.h"

class ProfilerStatusManager {
	HANDLE mutex;
	HANDLE hStatusWrittenEvent;
	ProfilerStatus* currentStatus;
	ProfilerStatus previousStatus;
	int* currentMethod;
	int previousMethod;

	float lastSpeedup;
	int lastMethodIndex;

	DelayCalculator calc;
	ResultsHandler resultsHandler;

public:
	ProfilerStatusManager(string& _processName);
	ProfilerStatus getCurrentStatus();
	ProfilerStatus getPreviousStatus();
	int getCurrentMethod();
	int getPreviousMethod();
	void setStatus(ProfilerStatus _new);
	void announceStatusChange();
};