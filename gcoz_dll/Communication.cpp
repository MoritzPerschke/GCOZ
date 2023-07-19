#include "Communication.h"

Communication::Communication(){
	hDllFileMapping = OpenFileMappingA(
		FILE_MAP_WRITE,
		FALSE,
		"gcoz_dll"
	); if (hDllFileMapping == NULL) {
		SetLastError(1);
	}

	hProfilerFileMapping = OpenFileMappingA(
		FILE_MAP_READ,
		FALSE,
		"gcoz_profiler"
	); if (hProfilerFileMapping == NULL) {
		SetLastError(1);
	}

	pSharedMemoryDll = MapViewOfFile(
		hDllFileMapping,
		FILE_MAP_WRITE,
		0, 0, 0
	); if (pSharedMemoryDll == NULL) {
		SetLastError(1);
	}

	pSharedMemoryProfiler = MapViewOfFile(
		hProfilerFileMapping,
		FILE_MAP_READ,
		0, 0, 0
	); if (pSharedMemoryProfiler == NULL) {
		SetLastError(1);
	}

	hDllWrittenEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, "dllWrittenEvent");
	hProfilerWrittenEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, "profilerWrittenEvent");

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

ProfilerMessage Communication::getMessage(DWORD waitTimeout) {
	DWORD dWaitResult = WaitForSingleObject(hProfilerWrittenEvent, waitTimeout);
	ProfilerMessage profilerMessage;

	if (dWaitResult == WAIT_OBJECT_0) {
		profilerMessage = *pProfilerData;
	}
	else {
		profilerMessage.valid = false;
	}

	return profilerMessage;
}