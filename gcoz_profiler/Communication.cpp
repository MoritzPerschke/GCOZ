#include "Communication.h"

Communication::Communication() {
	hDllFileMapping = CreateFileMappingA(
		INVALID_HANDLE_VALUE,
		FALSE,
		PAGE_READWRITE,
		0,
		2048,
		"gcoz_dll"
	); if (hDllFileMapping == NULL) {
		SetLastError(1);
	}

	hProfilerFileMapping = CreateFileMappingA(
		INVALID_HANDLE_VALUE,
		FALSE,
		PAGE_READWRITE,
		0,
		2048,
		"gcoz_profiler"
	); if (hProfilerFileMapping == NULL) {
		SetLastError(1);
	}

	pSharedMemoryDll = MapViewOfFile(
		hDllFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, 0
	); if (pSharedMemoryDll == NULL) {
		SetLastError(1);
	}

	pSharedMemoryProfiler = MapViewOfFile(
		hProfilerFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, 0
	); if (pSharedMemoryProfiler == NULL) {
		SetLastError(1);
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