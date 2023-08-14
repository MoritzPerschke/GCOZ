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
		std::cout << ok << "Measured method durations:" << std::endl;
		for (int i = 0; i < _dllMsg.durations.size(); i++) {
			if (_dllMsg.durations[i].count() > 0) {
				std::cout << inf << std::setw(3) << std::setfill(' ') << i << ": " << std::setw(7) << _dllMsg.durations[i].count() << std::endl;
			}
		}

		calc.addBaseline(_dllMsg.durations, _dllMsg.frameTimes);
		newMessage(_nextMsg);
		break;

	case ProfilerStatus::GCOZ_PROFILE :
		for (int i = 0; i < MEASURE_FRAME_COUNT; i++) {
			newFtAverages += _dllMsg.frameTimes[i].count() / _dllMsg.frameTimes.size();
		}
		std::cout << ok << "Difference in Frametime averages: " << newFtAverages << " - " << calc.getBaselineFt() << " = " << newFtAverages - calc.getBaselineFt() << std::endl;

		calc.addResult(_dllMsg.frameTimes);
		newMessage(_nextMsg);
		break;
	default :
		throw std::runtime_error("Unexpected Status in last Message from dll");
	}
	return _nextMsg.status;
}