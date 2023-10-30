#pragma once

#include <Windows.h>
#include <spdlog/spdlog.h>
#include <ProfilerStatus.h>
#include <Messages.h>
#include <SharedMemoryBoost.hpp>
#include <functional>
#include <utility>

class Communication {

	HANDLE hDllFileMapping, hProfilerFileMapping;
	LPVOID pSharedMemoryDll, pSharedMemoryProfiler;
	HANDLE hDllWrittenEvent, hProfilerWrittenEvent, hDllDataReceived, hProfilerDataReceived;
	ProfilerMessage* pProfilerData;

	IPC::DurationVector* durationMap;

	// this should ensure that on creation/destruction of the Communication object the shared memory is properly destroyed
	// https://www.boost.org/doc/libs/1_83_0/doc/html/interprocess/quick_guide.html#interprocess.quick_guide.qg_interprocess_container
	struct shm_remove {
		shm_remove() { boost::interprocess::shared_memory_object::remove("gcoz_FrametimesShared"); }
		~shm_remove() { boost::interprocess::shared_memory_object::remove("gcoz_FrametimesShared"); }
	}remover;

public:
	~Communication();
	void init();

	void setupBoostShared();
	void readBoostShared();

	bool sendMessage(ProfilerMessage _msg);
	DWORD waitMsg();
	DWORD waitRecv();
	Measurement getMeasurement();
	Result getResult();
	ThreadIDMessage getThreadIDs();
};