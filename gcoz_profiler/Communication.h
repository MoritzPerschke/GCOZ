#pragma once

#include <Windows.h>
#include <spdlog/spdlog.h>
#include <ProfilerStatus.h>
#include <Messages.h>
#include <SharedMemoryBoost.hpp>

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