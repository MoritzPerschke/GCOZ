#pragma once

#include <Windows.h>
#include "Messages.h"
#include "ResultsHandler.h"
#include "DelayCalculator.h"

class MessageHandler {
	DelayCalculator calc;
	ResultsHandler resH;

	ProfilerMessage response;

	float lastSpeedup;
	int lastMethod;

public:
	MessageHandler(string& _processName);
	ProfilerStatus nextStatus();
	void nextMessage(ProfilerMessage& _msg, ProfilerStatus _status);
	void handleMeasurement(Measurement _measurement);
	void handleResult(Result _result);
	void finish();
	//void handleIDs();
};