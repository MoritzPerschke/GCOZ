#pragma once
#include <Windows.h>
#include <iostream>
#include "Messages.h"

// maybe singleton? https://stackoverflow.com/a/1008289/15005309
class Communication {
	HANDLE hDllFileMapping, hProfilerFileMapping;
	LPVOID pSharedMemoryDll, pSharedMemoryProfiler;

public:
	HANDLE hDllWrittenEvent, hProfilerWrittenEvent, hDllDataReceived, hProfilerDataReceived;
	ProfilerMessage* pProfilerData;
	DllMessage* pDllData;
	~Communication();
	void init();
	DllMessage getMessage();
	bool sendMessage(ProfilerMessage _msg);
};