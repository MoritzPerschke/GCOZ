#pragma once
#include <Windows.h>
#include <string>
#include "ErrorMessage.h"
#include "../gcoz_profiler/Messages.h"

// maybe singleton? https://stackoverflow.com/a/1008289/15005309
class Communication {
	HANDLE hDllFileMapping, hProfilerFileMapping;
	LPVOID pSharedMemoryDll, pSharedMemoryProfiler;

public:
	HANDLE hDllWrittenEvent, hProfilerWrittenEvent, hDllDataReceived, hProfilerDataReceived;
	ProfilerMessage* pProfilerData;
	DllMessage* pDllData;

	Communication();
	~Communication();

	ProfilerMessage getMessage();
	bool newDataAvailable();
	bool sendMeasurement(const Measurement& _msg);
	bool sendResult(const Result& _msg);
};