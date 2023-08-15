#include "ProfilerStatusManager.h"

void ProfilerStatusManager::newMessage(ProfilerMessage& _msg) {
	_msg.status = ProfilerStatus::GCOZ_PROFILE;
	calc.calculateDelays(_msg.delays);
	_msg.valid = true;
}

ProfilerStatus ProfilerStatusManager::next(DllMessage _dllMsg, ProfilerMessage& _nextMsg) {
		long long int newFtAverages = 0;
	switch (_dllMsg.lastStatus) {
	case ProfilerStatus::GCOZ_MEASURE :
		calc.addBaseline(_dllMsg.durations, _dllMsg.frameTimes);
		newMessage(_nextMsg);
		break;

	case ProfilerStatus::GCOZ_PROFILE :
		calc.addResult(_dllMsg.frameTimes);
		newMessage(_nextMsg);
		break;
	default :
		throw std::runtime_error("Unexpected Status in last Message from dll");
	}
	return _nextMsg.status;
}