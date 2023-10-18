#include "Communication.h"

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

}

Communication::~Communication() {
	UnmapViewOfFile(hDllFileMapping);
	UnmapViewOfFile(hProfilerFileMapping);

	CloseHandle(pSharedMemoryDll);
	CloseHandle(pSharedMemoryProfiler);

	CloseHandle(hDllWrittenEvent);
	CloseHandle(hProfilerWrittenEvent);
	std::cout << "[*] Communication Destructor called" << std::endl;
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
