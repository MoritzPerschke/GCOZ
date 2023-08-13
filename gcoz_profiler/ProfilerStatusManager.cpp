#include "ProfilerStatusManager.h"

void ProfilerStatusManager::newMessage(ProfilerMessage& _msg) {
	_msg.status = ProfilerStatus::GCOZ_PROFILE;
	calc.calculateDelays(_msg.delays);
	_msg.valid = true;
}

ProfilerStatus ProfilerStatusManager::next(DllMessage _dllMsg, ProfilerMessage& _nextMsg) {
	switch (_dllMsg.lastStatus) {
	case ProfilerStatus::GCOZ_MEASURE :
		std::cout << ok << "Measured method durations:" << std::endl;
		for (int i = 0; i < _dllMsg.durations.size(); i++) {
			if (_dllMsg.durations[i].count() > 0) {
				std::cout << inf << i << ": " << _dllMsg.durations[i].count() << std::endl;
			}
		}

		calc.addBaseline(_dllMsg.durations, _dllMsg.frameTimes);
		newMessage(_nextMsg);
		break;

	case ProfilerStatus::GCOZ_PROFILE :
		std::cout << ok << "FrameDurations: " << std::endl;
		for (int i = 0; i < MEASURE_FRAME_COUNT; i++) {
			std::cout << inf << i << ": " << _dllMsg.frameTimes[i].count() << std::endl;
		}

		calc.addResult(_dllMsg.frameTimes);
		newMessage(_nextMsg);
		break;
	default :
		throw std::runtime_error("Unexpected Status in last Message from dll");
	}
	return _nextMsg.status;
}