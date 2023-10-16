#pragma once

#include <Windows.h>
#include "Messages.h"
#include "ResultsHandler.h"
#include "DelayCalculator.h"
#include "IdCollector.h"

class MessageHandler {
	DelayCalculator calc;
	IdCollector idsC;
	ResultsHandler resH;

	ProfilerMessage response;

	float lastSpeedup;
	int lastMethod;

public:
	MessageHandler(string& _processName);
	ProfilerStatus nextStatus();
	void nextMessage(ProfilerStatus _status, ProfilerMessage& _msg);
	void handleMeasurement(Measurement _measurement);
	void handleResult(Result _result);
	void handleThreadIDs(ThreadIDMessage _ids);
	void finish();
};