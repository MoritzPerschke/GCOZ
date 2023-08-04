#include "Communication.h"

void Communication::init() {
	hDllFileMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		2048,
		L"gcoz_dll_shared_memory"
	); 
	if (hDllFileMapping == NULL) {
		throw std::runtime_error("hDllFileMapping creation failed");
	}

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

	pSharedMemoryDll = MapViewOfFile(
		hDllFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, 0
	); if (pSharedMemoryDll == NULL) {
		throw std::runtime_error("pSharedMemoryDll File Mapping failed");
	}

	pSharedMemoryProfiler = MapViewOfFile(
		hProfilerFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, 0
	); if (pSharedMemoryProfiler == NULL) {
		throw std::runtime_error("pSharedMemoryProfiler File Mapping failed");
	}

	hDllWrittenEvent = CreateEventA(NULL, FALSE, FALSE, "dllWrittenEvent");
	hProfilerWrittenEvent = CreateEventA(NULL, FALSE, FALSE, "profilerWrittenEvent");

	pDllData = static_cast<DllMessage*>(pSharedMemoryDll);
	pProfilerData = static_cast<ProfilerMessage*>(pSharedMemoryProfiler);
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

DllMessage Communication::getMessage() {
	DWORD dWaitResult = WaitForSingleObject(hDllWrittenEvent, 60000);
	DllMessage dllMessage;
	
	if (dWaitResult == WAIT_OBJECT_0) {
		dllMessage = *pDllData;
		return dllMessage;
	}
	dllMessage.valid = false;
	return dllMessage;
}

bool Communication::sendMessage(ProfilerMessage _msg) {
	*pProfilerData = _msg;
	return SetEvent(hProfilerWrittenEvent);
}