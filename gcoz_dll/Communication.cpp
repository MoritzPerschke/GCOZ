#include "Communication.h"

Communication::Communication(){
	hDllFileMapping = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		L"gcoz_dll_shared_memory"
	); if (hDllFileMapping == NULL) {
		DisplayErrorBox(L"OpenFileMapping Dll");
	}

	hProfilerFileMapping = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		L"gcoz_profiler_shared_memory"
	); if (hProfilerFileMapping == NULL) {
		DisplayErrorBox(L"OpenFileMapping Profiler");
	}

	pSharedMemoryDll = MapViewOfFile( // this is the only one failing
		hDllFileMapping,
		FILE_MAP_WRITE, // the writing is the issue
		0, 0, 20
	); if (pSharedMemoryDll == NULL) {
		DisplayErrorBox(L"MapViewOfFile Dll");
	}

	pSharedMemoryProfiler = MapViewOfFile(
		hProfilerFileMapping,
		FILE_MAP_READ,
		0, 0, 2048
	); if (pSharedMemoryProfiler == NULL) {
		DisplayErrorBox(L"MapViewOfFile Profiler");
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

/* get Message from profiler, _waitTimeout in ms */
ProfilerMessage Communication::getMessage(DWORD _waitTimeout) {
	DWORD dWaitResult = WaitForSingleObject(hProfilerWrittenEvent, _waitTimeout);
	ProfilerMessage profilerMessage;

	if (dWaitResult == WAIT_OBJECT_0) {
		profilerMessage = *pProfilerData;
	}
	else {
		profilerMessage.valid = false;
	}

	return profilerMessage;
}

bool Communication::sendMessage(DllMessage _msg) {
	*pDllData = _msg;
	return SetEvent(hDllWrittenEvent);
}