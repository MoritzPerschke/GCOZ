#pragma once
#include <Windows.h>
#include "Messages.h"

class Communication {
	HANDLE hDllFileMapping, hProfilerFileMapping;
	LPVOID pSharedMemoryDll, pSharedMemoryProfiler;

public:
	HANDLE hDllWrittenEvent, hProfilerWrittenEvent;
	Message *pDllData, *pProfilerData;
	Communication();
	~Communication();

	Message getMessage();
};