#include "ProfilerStatusManager.h"

void ProfilerStatusManager::newMessage(ProfilerMessage& _msg) {
	_msg.status = ProfilerStatus::GCOZ_PROFILE;
	calc.calculateDelays(lastSpeedup, lastMethodIndex, _msg.delays);
	_msg.valid = true;
}

ProfilerStatus ProfilerStatusManager::next(DllMessage _dllMsg, ProfilerMessage& _nextMsg) {
	switch (_dllMsg.lastStatus) {
	case ProfilerStatus::GCOZ_MEASURE :
		resultsHandler.exportBaseline(_dllMsg.frameTimes);
		calc.addBaseline(_dllMsg.durations, _dllMsg.frameTimes);
		newMessage(_nextMsg);
		break;

	case ProfilerStatus::GCOZ_PROFILE :
		if (!calc.dataCollectedAllMethods()) {
			resultsHandler.addResultAll(_dllMsg.frameTimes, lastSpeedup);
		}
		else {
			resultsHandler.addResultSingle(_dllMsg.frameTimes, lastMethodIndex, lastSpeedup);
		}
		calc.addResult(_dllMsg.frameTimes);

		if (!calc.dataCollected()) { 
			newMessage(_nextMsg);
		}
		else {
			resultsHandler.exportResults();
		}
		break;

	default :
		throw std::runtime_error("Unexpected Status in last Message from dll");
	}
	return _nextMsg.status;
}