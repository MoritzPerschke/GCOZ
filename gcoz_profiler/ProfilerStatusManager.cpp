#include "ProfilerStatusManager.h"

ProfilerStatusManager::ProfilerStatusManager(string& _processName){
		resultsHandler = ResultsHandler(_processName);
		calc = DelayCalculator(resultsHandler);
		ids = IdCollector(); // prob revert this
}

void ProfilerStatusManager::nextMessage(ProfilerMessage& _msg) {
	std::cout << "Ids done: " << ids.isDone() << std::endl;
	if (!ids.isDone()) {
		_msg.status = ProfilerStatus::GCOZ_COLLECT_THREAD_IDS;
		_msg.methodID = ids.nextMethod();
	}
	else if (!calc.dataCollected()) {
		_msg.status = ProfilerStatus::GCOZ_PROFILE;
		calc.calculateDelays(lastSpeedup, lastMethodIndex, _msg.delays);
		_msg.valid = true;
	}
	else {
		std::cout << "[ProfilerStatusManaer::nextMessage]: IDs: " << ids.isDone() << "; calc: " << calc.dataCollected() << std::endl;
		_msg.status = ProfilerStatus::GCOZ_WAIT;
		_msg.valid  = true;
	}
	std::cout << inf << "Next status: " << _msg.status << std::endl;
}

ProfilerStatus ProfilerStatusManager::next(DllMessage _dllMsg, ProfilerMessage& _nextMsg) {
	switch (_dllMsg.lastStatus) {
	case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS:
		ids.addIDs(_dllMsg.threadIDs);
		if (ids.isDone()) {
			ids.finish();
		}
		nextMessage(_nextMsg);
		break;

	case ProfilerStatus::GCOZ_MEASURE_METHODS:
		calc.addBaseline(_dllMsg.durations, _dllMsg.frameTimes, _dllMsg.methodCalls);
		nextMessage(_nextMsg);
		break;

	case ProfilerStatus::GCOZ_PROFILE :
		if (!calc.dataCollectedAllMethods()) {
			resultsHandler.addResultAll(_dllMsg.frameTimes, _dllMsg.frameRates, lastSpeedup);
		}
		else {
			resultsHandler.addResultSingle(_dllMsg.frameTimes, _dllMsg.frameRates, lastMethodIndex, lastSpeedup);
		} // if delays on all methods done
		calc.measurementDone();

		if (!calc.dataCollected()) { 
			nextMessage(_nextMsg);
		}
		else {
			nextMessage(_nextMsg);
		} // if data collection complete
		break;

	default :
		throw std::runtime_error("Unexpected Status in last Message from dll");
	}
	return _nextMsg.status;
}

void ProfilerStatusManager::finish(){
	resultsHandler.exportResults();
}
