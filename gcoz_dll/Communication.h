#pragma once
#include <Windows.h>
#include "../gcoz_profiler/Messages.h"

// maybe singleton? https://stackoverflow.com/a/1008289/15005309
class Communication {
	HANDLE hDllFileMapping, hProfilerFileMapping;
	LPVOID pSharedMemoryDll, pSharedMemoryProfiler;

public:
	HANDLE hDllWrittenEvent, hProfilerWrittenEvent;
	ProfilerMessage* pProfilerData;
	DllMessage* pDllData;
	Communication();
	~Communication();

	ProfilerMessage getMessage(DWORD waitTimeout);
};