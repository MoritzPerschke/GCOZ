#include "ProfilerStatusManager.h"

ProfilerStatusManager::ProfilerStatusManager(string& _processName){
	resultsHandler = ResultsHandler(_processName);
}

void ProfilerStatusManager::nextMessage(ProfilerMessage& _msg) {
	_msg.status = ProfilerStatus::GCOZ_PROFILE;
	calc.calculateDelays(lastSpeedup, lastMethodIndex, _msg.delays);
	_msg.valid = true;
}

void ProfilerStatusManager::finalMessage(ProfilerMessage& _msg) {
	_msg.status = ProfilerStatus::GCOZ_WAIT;
	_msg.valid = true;
}

ProfilerStatus ProfilerStatusManager::next(DllMessage _dllMsg, ProfilerMessage& _nextMsg) {
	switch (_dllMsg.lastStatus) {
	case ProfilerStatus::GCOZ_MEASURE:
		resultsHandler.addBaseline(_dllMsg.frameTimes, _dllMsg.durations, _dllMsg.methodCalls);
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
			finalMessage(_nextMsg);
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
