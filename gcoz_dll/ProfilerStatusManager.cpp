#include "ProfilerStatusManager.h"

/*
	HANDLE mutex;
	HANDLE hStatusWrittenEvent;
	HANDLE hProfilingDataRecv;
	LPVOID pSharedMemoryStatus;
	*/

ProfilerStatusManager::ProfilerStatusManager(){
	/* shared memory for status */
	HANDLE hStatusFileMapping = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		L"gcoz_status_shared_memory"
	); if (hStatusFileMapping == NULL) { DisplayErrorBox(L"ProfilerStatusManager", L"hStatusFileMapping == NULL"); }
	
	LPVOID SharedMemoryStatus = MapViewOfFile(
		hStatusFileMapping,
		FILE_MAP_WRITE,
		0, 0, sizeof(ProfilerStatus)
	); if (SharedMemoryStatus == NULL) { DisplayErrorBox(L"ProfilerStatusManager", L"SharedMemoryStatus == NULL"); }
	pSharedMemoryStatus = static_cast<ProfilerStatus*>(SharedMemoryStatus);

	/* shared mutex */
	mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"SharedMemoryMutex");

	/* shared memory for current method */
	HANDLE hMethodFileMapping = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		L"gcoz_method_shared_memory"
	); if (hStatusFileMapping == NULL) { DisplayErrorBox(L"ProfilerStatusManager", L"hMethodFileMapping == NULL"); }
	
	LPVOID SharedMemoryMethod = MapViewOfFile(
		hMethodFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, sizeof(int)
	); if (hStatusFileMapping == NULL) { DisplayErrorBox(L"ProfilerStatusManager", L"SharedMemoryMethod == NULL"); }
	pSharedMemoryMethod = static_cast<int*>(SharedMemoryMethod);
	
	/* Synchronization Event */
	hStatusWrittenEvent = OpenEventA(NULL, FALSE, "hStatusWrittenEvent");
}

ProfilerStatus ProfilerStatusManager::getStatus() {
	return *pSharedMemoryStatus;
}

void ProfilerStatusManager::changeStatus(ProfilerStatus _newStatus){
	WaitForSingleObject(mutex, 10);
	*pSharedMemoryStatus = _newStatus;
	ReleaseMutex(mutex);
}

void ProfilerStatusManager::setStatus(ProfilerStatus _status) {
	*pSharedMemoryStatus = _status;
}

void ProfilerStatusManager::announceStatusChange() {
	SetEvent(hStatusWrittenEvent);
}

void ProfilerStatusManager::waitNewStatus(){
	WaitForSingleObject(hStatusWrittenEvent, INFINITE);
}

int ProfilerStatusManager::getMethod(){
	return *pSharedMemoryMethod;
}
