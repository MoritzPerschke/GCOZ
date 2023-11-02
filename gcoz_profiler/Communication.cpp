#include "Communication.h"

#include <vector>
/* Setup boost shared memory for:
- Frametimes
- method Durations
- how often methods get called
- calling Thread IDs for methods
- framerates
- delays
- maybe also current method ID for threadID collection
*/

void Communication::init() {
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

	/* Boost shared memory setup */
	shared_memory_object::remove("gcoz_SharedMemory");
	managed_shared_memory segment(create_only, "gcoz_SharedMemory", 65536);

	named_mutex generalMutex(create_only, "gcoz_SharedMemory_General_Mutex");

	/* Boost shared memory for measured method durations */
	const IPC::DurationMapValue_Allocator durationAlloc(segment.get_segment_manager());
	methodDurationsMap = segment.construct<IPC::DurationVector_Map>("Durations_Map")(std::less<int>(), durationAlloc);
	named_mutex durationsMutex(create_only, "gcoz_Durations_Map_Mutex");

	/* Boost shared memory for Thread ID collection */
	const IPC::ThreadIdVector_Map threadMapAlloc(segment.get_segment_manager());
	threadIDmap = segment.construct<IPC::ThreadIdVector_Map>("ThreadID_Map")(std::less<idHash>(), threadMapAlloc);
	named_mutex threadidMutex(create_only, "gcoz_ThreadID_Map_Mutex");

	/* Boost shared memory for Profiling Results */
	const IPC::ResultsMapValue_Allocator resultsMapAlloc(segment.get_segment_manager());
	profilingResultsMap = segment.construct<IPC::Results_Map>("Results_Map")(std::less<int>(), resultsMapAlloc); // this fails compile-time
	named_mutex resultsMutex(create_only, "gcoz_Results_Map_Mutex");

	// signal that setup is done
	HANDLE boostSharedCreated = CreateEventA(NULL, TRUE, TRUE, "gcoz_SharedMemory_ThreadIDs_created");

	/* Synchronization Events */
	hDllWrittenEvent = CreateEventA(NULL, FALSE, FALSE, "gcoz_DllWrittenEvent");
	hDllDataReceived = CreateEventA(NULL, FALSE, FALSE, "gcoz_DllDataReceived");
	hProfilerWrittenEvent = CreateEventA(NULL, FALSE, FALSE, "gcoz_ProfilerWrittenEvent");
	hProfilerDataReceived = CreateEventA(NULL, FALSE, FALSE, "gcoz_ProfilerDataReceived");

}

Communication::~Communication() {
	UnmapViewOfFile(hDllFileMapping);
	UnmapViewOfFile(hProfilerFileMapping);

	CloseHandle(pSharedMemoryDll);
	CloseHandle(pSharedMemoryProfiler);

	CloseHandle(hDllWrittenEvent);
	CloseHandle(hProfilerWrittenEvent);
	spdlog::info("Communication Destructor called");
}

Measurement Communication::getMeasurement() {
	Measurement* shared = static_cast<Measurement*>(pSharedMemoryDll);
	Measurement result = *shared;
	if (result.valid) {
		SetEvent(hProfilerDataReceived);
	}
	return result;
}

Result Communication::getResult() {
	Result* shared = static_cast<Result*>(pSharedMemoryDll);
	Result result = *shared;
	if (result.valid) {
		SetEvent(hProfilerDataReceived);
	}
	return result;
}

ThreadIDMessage Communication::getThreadIDs(){
	ThreadIDMessage* shared = static_cast<ThreadIDMessage*>(pSharedMemoryDll);
	ThreadIDMessage ids = *shared;
	if (ids.valid) {
		SetEvent(hProfilerDataReceived);
	}
	return ids;
}

bool Communication::sendMessage(ProfilerMessage _msg) {
	*pProfilerData = _msg;
	return SetEvent(hProfilerWrittenEvent);
}

DWORD Communication::waitMsg(){
	DWORD result = WaitForSingleObject(hDllWrittenEvent, INFINITE);
	return result;
}

DWORD Communication::waitRecv(){
	return WaitForSingleObject(hDllDataReceived, INFINITE); // infinite wait might be problematic
}
