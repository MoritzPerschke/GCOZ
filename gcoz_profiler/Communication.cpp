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
void Communication::setupBoostShared() {

	using namespace boost::interprocess;
	spdlog::info("Creating shared Memory for communication");

	shared_memory_object::remove("gcoz_FrametimesShared");
	managed_shared_memory segment(create_only, "gcoz_FrametimesShared", 65536);

	const IPC::MapValueAllocator mapAlloc(segment.get_segment_manager());
	const IPC::DurationAllocator durAlloc(segment.get_segment_manager());

	IPC::DurationMap* map = segment.construct<IPC::DurationMap>("Frametime_Map")(std::less<int>(), mapAlloc);
	for (int i = 0; i < 10; i++) {
		spdlog::info("test");
		map->insert(IPC::DurationMapType(i, IPC::DurationVector(durAlloc)));
	}
	for (int i = 0; i < 50; i++) {
		map->at(i % 10).push_back(i);
	}

	spdlog::info("shared mem created");
}

void Communication::readBoostShared() {
	using namespace boost::interprocess;
	spdlog::info("Reading memory");

	managed_shared_memory segment(open_only, "gcoz_FrametimesShared");

	IPC::DurationMap* map = segment.find<IPC::DurationMap>("Frametime_Map").first;

	for (const auto& elem : map->at(1)) {
		spdlog::info("value: {}", elem);
	}
}

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

	/* Dll -> Profiler Communication */
	hDllFileMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		16384,
		L"gcoz_dll_shared_memory"
	); 
	if (hDllFileMapping == NULL) {
		throw std::runtime_error("hDllFileMapping creation failed");
	}
	pSharedMemoryDll = MapViewOfFile(
		hDllFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, 0
	); if (pSharedMemoryDll == NULL) {
		throw std::runtime_error("pSharedMemoryDll File Mapping failed");
	}

	/* Synchronization Events */
	hDllWrittenEvent = CreateEventA(NULL, FALSE, FALSE, "gcoz_DllWrittenEvent");
	hDllDataReceived = CreateEventA(NULL, FALSE, FALSE, "gcoz_DllDataReceived");
	hProfilerWrittenEvent = CreateEventA(NULL, FALSE, FALSE, "gcoz_ProfilerWrittenEvent");
	hProfilerDataReceived = CreateEventA(NULL, FALSE, FALSE, "gcoz_ProfilerDataReceived");

	setupBoostShared();
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
