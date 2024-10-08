#pragma once

#include <Windows.h>
#include <spdlog/spdlog.h>

#include <ProfilerStatus.h>
#include <Messages.h>

#include "DelayCalculator.h"
#include "IdCollector.h"
#include "ResultsHandler.h"

/* Class providing the control logic for the overall profiling process */
class ProfilerStatusManager {
	// Components that provide the needed functionality
	DelayCalculator _delays;
	IdCollector		_ids;
	ResultsHandler  _results;

	// mutex used while writing status to shared memory
	HANDLE _statusMutex;
	// handle to the event signaling that a new status was written
	HANDLE _hStatusWrittenEvent;

	ProfilerStatus  _nextStatus;
	ProfilerStatus* _currentStatus;
	ProfilerStatus  _previousStatus;

	int* _currentMethod;
	int  _previousMethod;

	int* _currentDelay;
	int _previousDelay;

public:
	ProfilerStatusManager(std::string _processName);
	ProfilerStatus getCurrentStatus();
	ProfilerStatus getPreviousStatus();
	ProfilerStatus getNextStatus();
	void nextMessage(ProfilerStatus _status, ProfilerMessage& _msg);

	int   getCurrentMethod();
	int getCurrentDelay();

	void setCurrentMethod(int _method);

	void setStatus(ProfilerStatus _new);
	void announceStatusChange();

	void finish(std::string _outputName);
};