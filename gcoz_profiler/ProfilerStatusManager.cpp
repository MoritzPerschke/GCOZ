#include "ProfilerStatusManager.h"

ProfilerStatusManager::ProfilerStatusManager(){

	HANDLE hStatusFileMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(ProfilerStatus),
		L"gcoz_status_shared_memory"
	); if (hStatusFileMapping == NULL) {spdlog::error("File mapping creation for status failed");}
	LPVOID SharedMemoryStatus = MapViewOfFile(
		hStatusFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, sizeof(ProfilerStatus)
	); if (SharedMemoryStatus == NULL) {spdlog::error("Mapping view of Status file failed");}
	currentStatus = static_cast<ProfilerStatus*>(SharedMemoryStatus);
	// Shared memory for status sharing

	HANDLE hSharedMutexMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(HANDLE),
		L"SharedMemoryMutex"
	); if (hSharedMutexMapping == NULL) { spdlog::error("File mapping creation for mutex failed"); }
	mutex = CreateMutex(NULL, FALSE, L"SharedMemoryMutex");
	// shared memory for mutex

	HANDLE hMethodFileMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(int),
		L"gcoz_method_shared_memory"
	); if (hMethodFileMapping == NULL) { spdlog::error("File creation for method failed");}
	LPVOID SharedMemoryMethod = MapViewOfFile(
		hMethodFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, sizeof(int)
	); if (SharedMemoryMethod == NULL) { spdlog::error("Mapping view of Method file failed"); }
	currentMethod = static_cast<int*>(SharedMemoryMethod);
	// shared memory for method 

	hStatusWrittenEvent = CreateEventA(NULL, FALSE, FALSE, "hStatusWrittenEvent");
}

ProfilerStatus ProfilerStatusManager::getCurrentStatus() {
	return *currentStatus;
}

ProfilerStatus ProfilerStatusManager::getPreviousStatus()
{
	return previousStatus;
}

int ProfilerStatusManager::getCurrentMethod() {
	return *currentMethod;
}

void ProfilerStatusManager::setCurrentMethod(int _method){
	*currentMethod = _method;
}

void ProfilerStatusManager::setStatus(ProfilerStatus _new) {
	WaitForSingleObject(mutex, INFINITE);
	*currentStatus = _new;
	ReleaseMutex(mutex);
}

void ProfilerStatusManager::announceStatusChange(){
	SetEvent(hStatusWrittenEvent);
}
