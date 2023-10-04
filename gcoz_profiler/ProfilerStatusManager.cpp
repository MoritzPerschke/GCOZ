#include "ProfilerStatusManager.h"

ProfilerStatusManager::ProfilerStatusManager(string& _processName){
	resultsHandler = ResultsHandler(_processName);

	HANDLE hStatusFileMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(ProfilerStatus),
		L"gcoz_status_shared_memory"
	); if (hStatusFileMapping == NULL) {throw std::runtime_error("File mapping creation for status failed");}
	LPVOID SharedMemoryStatus = MapViewOfFile(
		hStatusFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, 0
	); if (SharedMemoryStatus == NULL) {throw std::runtime_error("Mapping view of Status file failed");}
	currentStatus = static_cast<ProfilerStatus*>(SharedMemoryStatus);
	// Shared memory for status sharing

	HANDLE hSharedMutexMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(HANDLE),
		L"SharedMemoryMutex"
	); if (hSharedMutexMapping == NULL) { throw std::runtime_error("File mapping creation for mutex failed"); }
	mutex = CreateMutex(NULL, FALSE, L"SharedMemoryMutex");
	// shared memory for mutex

	HANDLE hMethodFileMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(int),
		L"gcoz_method_shared_memory"
	); if (hMethodFileMapping == NULL) { throw std::runtime_error("File creation for method failed");}
	LPVOID SharedMemoryMethod = MapViewOfFile(
		hMethodFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, 0
	); if (SharedMemoryMethod == NULL) { throw std::runtime_error("Mapping view of Method file failed"); }
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

int ProfilerStatusManager::getPreviousMethod()
{
	return previousMethod;
}

void ProfilerStatusManager::setStatus(ProfilerStatus _new) {
	WaitForSingleObject(mutex, INFINITE);
	*currentStatus = _new;
	ReleaseMutex(mutex);
}

void ProfilerStatusManager::announceStatusChange(){
	SetEvent(hStatusWrittenEvent);
}
