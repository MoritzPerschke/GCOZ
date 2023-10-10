#pragma once

#include <Windows.h>
#include "Communication.h"
#include "ErrorMessage.h"
#include "../gcoz_profiler/ProfilerStatus.h"

class ProfilerStatusManager {
	HANDLE mutex;
	HANDLE hStatusWrittenEvent;
	Communication com;
	ProfilerStatus* pSharedMemoryStatus;
	int* pSharedMemoryMethod;

public:
	ProfilerStatusManager();
	ProfilerStatus getStatus();
	void changeStatus(ProfilerStatus _newStatus);
	void setStatus(ProfilerStatus _status);
	int getMethod();
};