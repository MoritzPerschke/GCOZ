#include "ProfilerStatusManager.h"

/*
	HANDLE mutex;
	HANDLE hStatusWrittenEvent;
	HANDLE hProfilingDataRecv;
	LPVOID pSharedMemoryStatus;
	*/

ProfilerStatusManager::ProfilerStatusManager(){
	HANDLE hStatusFileMapping = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		L"gcoz_status_shared_memory"
	);
	LPVOID SharedMemoryStatus = MapViewOfFile(
		hStatusFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, sizeof(ProfilerStatus)
	);
	pSharedMemoryStatus = static_cast<ProfilerStatus*>(SharedMemoryStatus);
	// shared memory for status

	mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"SharedMemoryMutex");
	// shared mutex

	HANDLE hMethodFileMapping = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		L"gcoz_method_shared_memory"
	);
	LPVOID SharedMemoryMethod = MapViewOfFile(
		hMethodFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, sizeof(int)
	);
	pSharedMemoryMethod = static_cast<int*>(SharedMemoryMethod);
	// shared memory method

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

int ProfilerStatusManager::getMethod(){
	return *pSharedMemoryMethod;
}
