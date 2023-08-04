#include "DelayManager.h"

DelayManager::DelayManager() {
	methodDelays.fill(0);
}

DWORD DelayManager::getDelay(int methodIndex){
	return methodDelays[methodIndex];
}

void DelayManager::updateDelays(std::array<DWORD, 205> _newDelays){
	methodDelays = _newDelays;
}

void DelayManager::resetDelays() {
	methodDelays.fill(0);
}