#pragma once
#include <Windows.h>
#include <string>
#include "ErrorMessage.h"
#include "../shared/Messages.h"

// maybe singleton? https://stackoverflow.com/a/1008289/15005309
class Communication {
	HANDLE hProfilerFileMapping;
	LPVOID pSharedMemoryProfiler;

public:
	HANDLE hDllDoneEvent, hProfilerWrittenEvent, hDllDataReceived, hProfilerDataReceived;
	ProfilerMessage* pProfilerData;

	Communication();
	~Communication();

	ProfilerMessage getMessage();
	void announceFinish();
	bool newDataAvailable();
};