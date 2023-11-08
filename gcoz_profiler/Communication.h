#pragma once

#include <Windows.h>
#include <spdlog/spdlog.h>
#include <ProfilerStatus.h>
#include <Messages.h>
#include <SharedMemoryBoost.hpp>
#include <functional>
#include <utility>

// this should ensure that on creation/destruction of the Communication object the shared memory is properly destroyed
// https://www.boost.org/doc/libs/1_83_0/doc/html/interprocess/quick_guide.html#interprocess.quick_guide.qg_interprocess_container
struct shm_remove {
	shm_remove() { boost::interprocess::shared_memory_object::remove("gcoz_FrametimesShared"); }
	~shm_remove() { boost::interprocess::shared_memory_object::remove("gcoz_FrametimesShared"); }
};

class Communication {

	shm_remove remover;
	HANDLE hProfilerFileMapping;
	LPVOID pSharedMemoryProfiler;
	HANDLE hDllWrittenEvent, hProfilerWrittenEvent, hDllDataReceived, hProfilerDataReceived;
	ProfilerMessage* pProfilerData;
	HANDLE dllDoneEvent;

	// these do not need to be member variables i think
	IPC::Results_Map* frameTimesMap; // FrameTimes_Map; gcoz_FrameTimes_Map_Mutex
	IPC::Results_Map* frameRatesMap; // Framerates_Map; gcoz_FrameRates_Map_Mutex
	IPC::DurationVector_Map* methodDurationsMap; // Durations_Map; gcoz_Durations_Map_Mutex
	IPC::ThreadIdVector_Map* threadIDmap; // ThreadID_Map; gcoz_ThreadID_Map_Mutex

public:
	Communication();
	~Communication();

	bool sendMessage(ProfilerMessage _msg);
	DWORD waitDllDone();
	//DWORD waitMsg();
	DWORD waitRecv();
};