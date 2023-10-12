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

	pSharedMemoryDll = MapViewOfFile(
		hDllFileMapping,
		FILE_MAP_WRITE,
		0, 0, 16384
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

	hDllWrittenEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, "gcoz_DllWrittenEvent");
	hDllDataReceived = OpenEventA(EVENT_ALL_ACCESS, FALSE, "gcoz_DllDataReceived");
	hProfilerWrittenEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, "gcoz_ProfilerWrittenEvent");
	hProfilerDataReceived = OpenEventA(EVENT_ALL_ACCESS, FALSE, "gcoz_ProfilerDataReceived");

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

bool Communication::newDataAvailable(){
	return WaitForSingleObject(hProfilerWrittenEvent, 0) == WAIT_OBJECT_0;
}

bool Communication::sendMeasurement(const Measurement& _msg) {
	if (pDllData != NULL) {
		Measurement* shared = static_cast<Measurement*> (pDllData);
		*shared = _msg; // EXCEPTION_ACCESS_VIOLATION, this is NULL?
	}
	else {
		DisplayErrorBox(L"[Communication.cpp::sendMeasurement] pDllData == NULL");
	}
	if (!SetEvent(hDllWrittenEvent)) {
		DisplayErrorBox(L"Failed to set hDllWrittenEvent");
	}
	return WaitForSingleObject(hProfilerDataReceived, 2) == WAIT_OBJECT_0; // not sure about delay here
}

bool Communication::sendResult(const Result& _msg) {
	if (pDllData != NULL) {
		Result* shared = static_cast<Result*> (pDllData);
		*shared = _msg; // EXCEPTION_ACCESS_VIOLATION, this is NULL?
	}
	else {
		DisplayErrorBox(L"[Communication.cpp::sendResult] pDllData == NULL");
	}
	if (!SetEvent(hDllWrittenEvent)) {
		DisplayErrorBox(L"Failed to set hDllWrittenEvent");
	}
	return WaitForSingleObject(hProfilerDataReceived, 2) == WAIT_OBJECT_0; // not sure about delay here
}
