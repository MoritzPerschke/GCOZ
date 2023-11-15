#include "Communication.h"

Communication::Communication(){
	hProfilerFileMapping = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		L"gcoz_profiler_shared_memory"
	); if (hProfilerFileMapping == NULL) {
		DisplayErrorBox(L"OpenFileMapping Profiler");
	}

	pSharedMemoryProfiler = MapViewOfFile(
		hProfilerFileMapping,
		FILE_MAP_READ,
		0, 0, 2048
	); if (pSharedMemoryProfiler == NULL) {
		DisplayErrorBox(L"MapViewOfFile Profiler");
	}

	hDllDoneEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, "gcoz_dllDone_Event");
	hDllDataReceived = OpenEventA(EVENT_ALL_ACCESS, FALSE, "gcoz_DllDataReceived");
	hProfilerWrittenEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, "gcoz_ProfilerWrittenEvent");
	hProfilerDataReceived = OpenEventA(EVENT_ALL_ACCESS, FALSE, "gcoz_ProfilerDataReceived");

	pProfilerData = static_cast<ProfilerMessage*>(pSharedMemoryProfiler);
	//DisplayInfoBox(L"Communcation", L"Construction success");
}

Communication::~Communication() {
	UnmapViewOfFile(hProfilerFileMapping);

	CloseHandle(pSharedMemoryProfiler);

	CloseHandle(hDllDoneEvent);
	CloseHandle(hProfilerWrittenEvent);
}

///* get Message from profiler, _waitTimeout in ms */
ProfilerMessage Communication::getMessage() {
	/* options:
	1. waitforsingleobject using timeout 0
	2. static profilermessage, update if static != *pProfilerData
	  current: straight read shared mem, check if new available elsewhere
	*/
	ProfilerMessage newData = *pProfilerData;
	SetEvent(hDllDataReceived);
	return newData;
}

void Communication::announceFinish() {
	SetEvent(hDllDoneEvent);
}

bool Communication::newDataAvailable(){
	return WaitForSingleObject(hProfilerWrittenEvent, 0) == WAIT_OBJECT_0;
}
