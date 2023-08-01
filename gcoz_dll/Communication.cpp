#include "Communication.h"

std::wstring GetLastErrorAsString()
{
	DWORD errorCode = GetLastError();
	LPWSTR messageBuffer = nullptr;
	size_t size = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&messageBuffer,
		0,
		NULL
	);

	std::wstring message(messageBuffer, size);

	LocalFree(messageBuffer);

	return message;
}

Communication::Communication(){
	hDllFileMapping = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		L"gcoz_dll_shared_memory"
	); if (hDllFileMapping == NULL) {
		MessageBoxW(0, GetLastErrorAsString().c_str(), L"OpenFileMapping Dll", MB_SETFOREGROUND);
	}

	hProfilerFileMapping = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		L"gcoz_profiler_shared_memory"
	); if (hProfilerFileMapping == NULL) {
		MessageBoxW(0, GetLastErrorAsString().c_str(), L"OpenFileMapping Profiler", MB_SETFOREGROUND);
	}

	pSharedMemoryDll = MapViewOfFile( // this is the only one failing
		hDllFileMapping,
		FILE_MAP_WRITE, // the writing is the issue
		0, 0, 20
	); if (pSharedMemoryDll == NULL) {
		
		MessageBoxW(0, GetLastErrorAsString().c_str(), L"MapViewOfFile Dll", MB_SETFOREGROUND);
	}

	pSharedMemoryProfiler = MapViewOfFile(
		hProfilerFileMapping,
		FILE_MAP_READ,
		0, 0, 2048
	); if (pSharedMemoryProfiler == NULL) {
		MessageBoxW(0, GetLastErrorAsString().c_str(), L"MapViewOfFile Profiler", MB_SETFOREGROUND);
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