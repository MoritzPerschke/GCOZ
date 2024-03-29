#pragma once

#include <Windows.h>
#include "Communication.h"
#include "ErrorMessage.h"
#include "../shared/ProfilerStatus.h"

class ProfilerStatusManager {
	HANDLE mutex;
	HANDLE hStatusWrittenEvent;
	Communication com;
	ProfilerStatus* pSharedMemoryStatus;
	int* pSharedMemoryMethod;
	int* pSharedMemoryDelay;

public:
	ProfilerStatusManager();
	ProfilerStatus getStatus();
	void changeStatus(ProfilerStatus _newStatus);
	void announceStatusChange();
	void waitNewStatus();
	void setStatus(ProfilerStatus _status);
	int getMethod();
	int getDelay();
};