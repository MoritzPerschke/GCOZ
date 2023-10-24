#pragma once
#include <Windows.h>
#include <iostream>
#include "../shared/ProfilerStatus.h"
#include "../shared/Messages.h"
#include <spdlog/spdlog.h>

// maybe singleton? https://stackoverflow.com/a/1008289/15005309
class Communication {
	HANDLE hDllFileMapping, hProfilerFileMapping;
	LPVOID pSharedMemoryDll, pSharedMemoryProfiler;
	HANDLE hDllWrittenEvent, hProfilerWrittenEvent, hDllDataReceived, hProfilerDataReceived;
	ProfilerMessage* pProfilerData;

public:
	~Communication();
	void init();

	bool sendMessage(ProfilerMessage _msg);
	DWORD waitMsg();
	DWORD waitRecv();
	Measurement getMeasurement();
	Result getResult();
	ThreadIDMessage getThreadIDs();
};