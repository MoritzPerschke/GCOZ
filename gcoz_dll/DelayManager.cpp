#include "DelayManager.h"

DelayManager::DelayManager() {
	methodDelays.fill(0);
}

DWORD DelayManager::getDelay(int methodIndex){
	return methodDelays[methodIndex];
}

void DelayManager::updateDelays(Communication com){
	ProfilerMessage msg = com.getMessage(INFINITE);
	if (msg.valid) {
		methodDelays = msg.delays;
	}
}
