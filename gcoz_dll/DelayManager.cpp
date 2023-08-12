#include "DelayManager.h"

DelayManager::DelayManager() {
	methodDelays.fill(0);
}

DWORD DelayManager::getDelay(int methodIndex){
	return methodDelays[methodIndex];
}

void DelayManager::updateDelays(std::array<DWORD, D3D11_METHOD_COUNT> _newDelays){
	methodDelays = _newDelays;
}

void DelayManager::resetDelays() {
	methodDelays.fill(0);
}