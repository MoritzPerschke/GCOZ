#pragma once

#include <Windows.h>
#include <shared_mutex>
#include "../shared/ProfilerStatus.h"
#include "ResultsHandler.h"
#include "DelayCalculator.h"
#include "../shared/Messages.h"
#include "IdCollector.h"
#include <spdlog/spdlog.h>

class ProfilerStatusManager {
	HANDLE mutex;
	HANDLE hStatusWrittenEvent;
	ProfilerStatus* currentStatus;
	ProfilerStatus previousStatus;
	int* currentMethod;
	int previousMethod;

	float lastSpeedup;
	int lastMethodIndex;

public:
	ProfilerStatusManager();
	ProfilerStatus getCurrentStatus();
	ProfilerStatus getPreviousStatus();
	int getCurrentMethod();
	void setCurrentMethod(int _method);
	void setStatus(ProfilerStatus _new);
	void announceStatusChange();
};