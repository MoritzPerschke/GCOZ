#pragma once

#include <Windows.h>
#include <spdlog/spdlog.h>
#include "../shared/ProfilerStatus.h"
#include "../shared/Messages.h"

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

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