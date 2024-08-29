#include "ProfilerStatusManager.h"

ProfilerStatusManager::ProfilerStatusManager(std::string _processName) {
	_results = ResultsHandler();

	// Setup shared memory for communicating current status of the profiler
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
	_currentStatus = static_cast<ProfilerStatus*>(SharedMemoryStatus);

	// shared memory for mutex to control writing to the above
	HANDLE hSharedMutexMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(HANDLE),
		L"SharedMemoryMutex"
	); if (hSharedMutexMapping == NULL) { spdlog::error("File mapping creation for mutex failed"); }
	_statusMutex = CreateMutex(NULL, FALSE, L"SharedMemoryMutex");

	// shared memory for currently profiled method
	HANDLE hMethodFileMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(int),
		L"gcoz_method_shared_memory"
	); if (hMethodFileMapping == NULL) { spdlog::error("File creation for method failed"); }

	LPVOID SharedMemoryMethod = MapViewOfFile(
		hMethodFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, sizeof(int)
	); if (SharedMemoryMethod == NULL) { spdlog::error("Mapping view of Method file failed"); }

	_currentMethod = static_cast<int*>(SharedMemoryMethod);

	// shared memory for communicating current 'speedup'
	HANDLE hDelayFileMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(float),
		L"gcoz_delay_shared_memory"
	); if (hDelayFileMapping == NULL) { spdlog::error("File creation for delay failed"); }

	LPVOID SharedMemoryDelay = MapViewOfFile(
		hDelayFileMapping,
		FILE_MAP_ALL_ACCESS,
		0, 0, sizeof(float)
	); if (SharedMemoryDelay == NULL) { spdlog::error("Mapping view of delay file failed"); }
	
	_currentDelay = static_cast<int*>(SharedMemoryDelay);

	_hStatusWrittenEvent = CreateEventA(NULL, FALSE, FALSE, "StatusWrittenEvent");
	if (_hStatusWrittenEvent == NULL) { spdlog::error("Failed to create status written event"); }
}

ProfilerStatus ProfilerStatusManager::getNextStatus(){
	/* Handle new data */
	switch (_previousStatus)
	{ //moving this here shrinks main function a lot
	case ProfilerStatus::GCOZ_MEASURE:
		_delays.addBaseline(_ids.getRefMethodQueue());
		break;
	case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS:
		_ids.idsAdded();
		break;
	case ProfilerStatus::GCOZ_PROFILE:
		if (!_delays.dataCollectedAllMethods()) {
			_delays.measurementDoneAll(*_currentDelay);
		}
		else {
			_delays.measurementDoneSingle(*_currentDelay, *_currentMethod);
		}
		break;
	default:
		break;
	}
	/*
	- are all baseline measurements collected?
	- are all slowdowns applied to all methods?
	- are all slowdowns applied to every method?
	- (are all threadIDs collected?)
	- is everything done?
	*/
	if (!_delays.isBaselineAdded()) 
	{ // no durations for methods in memory, measure them
		return ProfilerStatus::GCOZ_MEASURE;
	}
	else if (!_ids.isDone())
	{ // not all thread IDs in memory
		return ProfilerStatus::GCOZ_COLLECT_THREAD_IDS;
	}
	else if (!_delays.dataCollectedAllMethods())
	{ // not all baseline frametimes collected
		return ProfilerStatus::GCOZ_PROFILE;
	}
	else if (!_delays.dataCollected())
	{ // not all data collected
		return ProfilerStatus::GCOZ_PROFILE;
	}
	else
	{ // everything should be done
		return ProfilerStatus::GCOZ_FINISH;
	}
}

void ProfilerStatusManager::nextMessage(ProfilerStatus _status, ProfilerMessage& _msg){
	switch (_status) {
	case ProfilerStatus::GCOZ_MEASURE:
		_msg.delays.fill(Nanoseconds(0));
		break;
	case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS:
		//_msg.delays.fill(Nanoseconds(0));
		*_currentMethod = _ids.nextMethod();
		break;
	case ProfilerStatus::GCOZ_PROFILE:
		int delay;
		int method;
		_delays.calculateDelays(delay, method, _msg.delays);
		*_currentDelay = delay;
		*_currentMethod = method;
		_msg.valid = true;
		break;
	default:
		spdlog::error("No next message possible for status [{}]", profilerStatusString(_status));
	}
}

/* Status currently in memory, only useful during run, status changes do wait when dll done */
ProfilerStatus ProfilerStatusManager::getCurrentStatus() {
	return *_currentStatus;
}

/* Status during last run, status changes do wait when dll done*/
ProfilerStatus ProfilerStatusManager::getPreviousStatus()
{
	return _previousStatus;
}

int ProfilerStatusManager::getCurrentMethod() {
	return *_currentMethod;
}

int ProfilerStatusManager::getCurrentDelay(){
	return *_currentDelay;
}

void ProfilerStatusManager::setCurrentMethod(int _method){
	*_currentMethod = _method;
}

void ProfilerStatusManager::setStatus(ProfilerStatus _new) {
	_previousStatus = _new;
	WaitForSingleObject(_statusMutex, INFINITE);
	*_currentStatus = _new;
	ReleaseMutex(_statusMutex);
}

void ProfilerStatusManager::announceStatusChange(){
	SetEvent(_hStatusWrittenEvent);
}

void ProfilerStatusManager::finish(std::string _outputName){
	_results.exportResults(_outputName);
}
