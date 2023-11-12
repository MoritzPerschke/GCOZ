#include "Communication.h"

void boostCleanup() {
	shared_memory_object::remove("gcoz_SharedMemory");

	named_mutex::remove("gcoz_SharedMemory_General_Mutex");
	named_mutex::remove("gcoz_Durations_Map_Mutex");
	named_mutex::remove("gcoz_ThreadID_Map_Mutex");
	named_mutex::remove("gcoz_FrameTimes_Map_Mutex");
	named_mutex::remove("gcoz_FrameRates_Map_Mutex");
}
/* Setup boost shared memory for:
- Frametimes
- method Durations
- how often methods get called
- calling Thread IDs for methods
- framerates
- delays
- maybe also current method ID for threadID collection
*/
Communication::Communication() {
	/* Profiler -> Dll Communication */
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

	/* Make sure objects don't already exist */
	boostCleanup();
	/* Boost shared memory setup */
	managed_shared_memory segment(create_only, "gcoz_SharedMemory", 1024 * 2000); // 1KB * 1000 = 1MB
	named_mutex generalMutex(create_only, "gcoz_SharedMemory_General_Mutex");

	/* Boost shared memory for measured method durations */
	const IPC::DurationMapValue_Allocator durationAlloc(segment.get_segment_manager());
	//methodDurationsMap = segment.construct<IPC::DurationVector_Map>("Durations_Map")(std::less<int>(), durationAlloc);
	segment.construct<IPC::DurationVector_Map>("Durations_Map")(std::less<int>(), durationAlloc);
	named_mutex durationsMutex(create_only, "gcoz_Durations_Map_Mutex");
	spdlog::info("Building shared memory");

	/* Boost shared memory for Thread ID collection */
	const IPC::IdHashMapValue_Allocator threadMapAlloc(segment.get_segment_manager());
	//threadIDmap = segment.construct<IPC::ThreadIdVector_Map>("ThreadID_Map")(std::less<idHash>(), threadMapAlloc);
	segment.construct<IPC::ThreadIdVector_Map>("ThreadID_Map")(std::less<idHash>(), threadMapAlloc);
	named_mutex threadidMutex(create_only, "gcoz_ThreadID_Map_Mutex");

	/* Boost shared memory for Profiling Results */
	const IPC::ResultsMapValue_Allocator frameTimeAlloc(segment.get_segment_manager());
	//frameTimesMap = segment.construct<IPC::Results_Map>("FrameTimes_Map")(std::less<int>(), frameTimeAlloc);
	segment.construct<IPC::Results_Map>("FrameTimes_Map")(std::less<int>(), frameTimeAlloc);
	named_mutex frameTimesMutex(create_only, "gcoz_FrameTimes_Map_Mutex");

	const IPC::ResultsMapValue_Allocator frameRatesAlloc(segment.get_segment_manager());
	//frameRatesMap = segment.construct<IPC::Results_Map>("FrameRates_Map")(std::less<int>(), frameRatesAlloc);
	segment.construct<IPC::Results_Map>("FrameRates_Map")(std::less<int>(), frameRatesAlloc);
	named_mutex frameRatesMutex(create_only, "gcoz_FrameRates_Map_Mutex");

	/*---------[testing]----------*/

	//IPC::Duration_Allocator alloc = segment.get_segment_manager();
	//methodDurationsMap->insert(IPC::DurationMap_Type(1, IPC::Duration_Vector(alloc))); // this does not work either

	IPC::DurationMapValue_Allocator intAlloc = segment.get_segment_manager();
	IPC::Duration_Allocator duralloc = segment.get_segment_manager();
	IPC::DurationVector_Map* testing = segment.construct<IPC::DurationVector_Map>("testing")(intAlloc);
	testing->insert(IPC::DurationMap_Type(1, IPC::Duration_Vector(duralloc)));
	spdlog::info("here");


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

bool Communication::sendMessage(ProfilerMessage _msg) {
	*pProfilerData = _msg;
	//if (memcmp(pProfilerData, &_msg, sizeof(_msg)) == 0) {
	//	return SetEvent(hProfilerWrittenEvent);
	//}
	//else {
	//	return false;
	//} test later
	return SetEvent(hProfilerWrittenEvent);
}

DWORD Communication::waitDllDone() {
	return WaitForSingleObject(dllDoneEvent, INFINITE);
}

DWORD Communication::waitRecv(){
	return WaitForSingleObject(hDllDataReceived, INFINITE);
}

/*deprecated*/
//DWORD Communication::waitMsg(){
//	DWORD result = WaitForSingleObject(hDllWrittenEvent, INFINITE);
//	return result;
//}
