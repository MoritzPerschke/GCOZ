#pragma once

#include <Windows.h>
#include <spdlog/spdlog.h>
#include <ProfilerStatus.h>
#include <Messages.h>
#include <SharedMemoryBoost.hpp>
#include <functional>
#include <utility>

class Communication {

	// 
	HANDLE hProfilerFileMapping;
	LPVOID pSharedMemoryProfiler;
	HANDLE hDllWrittenEvent, hProfilerWrittenEvent, hDllDataReceived, hProfilerDataReceived;
	// Simple WinAPI shared memory for sending frame data
	ProfilerMessage* pProfilerData;
	HANDLE dllDoneEvent;

public:
	/* Setup boost shared memory for:
	- Frametimes
	- method Durations
	- Method call-count
	- calling Thread IDs for methods
	- framerates
	- delays
	*/
	Communication();
	// The explicit destructor is used to ensure proper
	// removal of all shared memory mappings
	~Communication();

	bool sendMessage(ProfilerMessage _msg);
	DWORD waitDllDone();
	DWORD waitRecv();
};