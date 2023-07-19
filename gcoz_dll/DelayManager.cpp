#include "DelayManager.h"

DelayManager::DelayManager() {
	methodDelays.fill(0);
}

DelayManager::DelayManager(Communication _com){
	com = _com;
	updateDelays();
}

DWORD DelayManager::getDelay(int methodIndex){
	return methodDelays[methodIndex];
}

void DelayManager::updateDelays(){
	ProfilerMessage msg = com.getMessage(INFINITE);
	if (msg.valid) {
		methodDelays = msg.delays;
	}
}
