#include "Communication.h"

void boostCleanup() {
	shared_memory_object::remove("gcoz_SharedMemory");

	named_mutex::remove("gcoz_SharedMemory_General_Mutex");
	named_mutex::remove("gcoz_Durations_Map_Mutex");
	named_mutex::remove("gcoz_ThreadID_Map_Mutex");
	named_mutex::remove("gcoz_FrameTimes_Map_Mutex");
	named_mutex::remove("gcoz_FrameRates_Map_Mutex");
}

Communication::Communication() {
	/* Profiler -> Dll Communication (to send calculated delays) */
	hProfilerFileMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		2048,
		L"gcoz_profiler_shared_memory"
	); if (hProfilerFileMapping == NULL) {
		throw std::runtime_error("hProfilerFileMapping creation failed");
	}
	pSharedMemoryProfiler = MapViewOfFile(
		hProfilerFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, 0
	); if (pSharedMemoryProfiler == NULL) {
		throw std::runtime_error("pSharedMemoryProfiler File Mapping failed");
	}
	pProfilerData = static_cast<ProfilerMessage*>(pSharedMemoryProfiler);

	/* Make sure boost objects don't already exist */
	boostCleanup();
	/* Boost shared memory general setup */
	managed_shared_memory segment(create_only, "gcoz_SharedMemory", 1024 * 50000); // 1KB * 1000 = 1MB
	named_mutex generalMutex(create_only, "gcoz_SharedMemory_General_Mutex");

	/* Boost shared memory for measured method durations */
	segment.construct<IPC::DurationVector_Map>("Durations_Map")(segment.get_segment_manager());
	named_mutex durationsMutex(create_only, "gcoz_Durations_Map_Mutex");
	spdlog::info("Building shared memory");

	/* Boost shared memory for Thread ID collection */
	segment.construct<IPC::ThreadIdVector_Map>("ThreadID_Map")(segment.get_segment_manager());
	named_mutex threadidMutex(create_only, "gcoz_ThreadID_Map_Mutex");

	/* Boost shared memory for Profiling Results */
	segment.construct<IPC::ResultsMap_Map>("FrameTimes_Map")(segment.get_segment_manager());
	named_mutex frameTimesMutex(create_only, "gcoz_FrameTimes_Map_Mutex");

	segment.construct<IPC::ResultsMap_Map>("FrameRates_Map")(segment.get_segment_manager());
	named_mutex frameRatesMutex(create_only, "gcoz_FrameRates_Map_Mutex");

	// signal that setup is done
	HANDLE boostSharedCreated = CreateEventA(NULL, TRUE, TRUE, "gcoz_SharedMemory_ThreadIDs_created");

	/* Synchronization Events */
	hDllWrittenEvent = CreateEventA(NULL, FALSE, FALSE, "gcoz_DllWrittenEvent");
	hDllDataReceived = CreateEventA(NULL, FALSE, FALSE, "gcoz_DllDataReceived");
	hProfilerWrittenEvent = CreateEventA(NULL, FALSE, FALSE, "gcoz_ProfilerWrittenEvent");
	hProfilerDataReceived = CreateEventA(NULL, FALSE, FALSE, "gcoz_ProfilerDataReceived");
	dllDoneEvent = CreateEventA(NULL, FALSE, FALSE, "gcoz_dllDone_Event");

}

Communication::~Communication() {
	UnmapViewOfFile(hProfilerFileMapping);

	CloseHandle(pSharedMemoryProfiler);

	CloseHandle(hDllWrittenEvent);
	CloseHandle(hProfilerWrittenEvent);
	spdlog::info("Communication Destructor called");
}

// Careful, this does not check for success(ful read)
bool Communication::sendMessage(ProfilerMessage _msg) {
	*pProfilerData = _msg;
	return SetEvent(hProfilerWrittenEvent);
}

DWORD Communication::waitDllDone() {
	return WaitForSingleObject(dllDoneEvent, INFINITE);
}

DWORD Communication::waitRecv(){
	return WaitForSingleObject(hDllDataReceived, INFINITE);
}
