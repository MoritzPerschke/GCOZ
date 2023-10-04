#pragma once
#include <Windows.h>
#include <iostream>
#include "ProfilerStatus.h"
#include "Messages.h"

// maybe singleton? https://stackoverflow.com/a/1008289/15005309
class Communication {
	HANDLE hDllFileMapping, hProfilerFileMapping;
	LPVOID pSharedMemoryDll, pSharedMemoryProfiler;
	HANDLE hDllWrittenEvent, hProfilerWrittenEvent, hDllDataReceived, hProfilerDataReceived;
	ProfilerMessage* pProfilerData;
	DllMessage* pDllData;

public:
	~Communication();
	void init();

	DllMessage getMessage(ProfilerStatus _status);
	bool sendMessage(ProfilerMessage _msg);
	DWORD waitMsg();
	DWORD waitRecv();
};