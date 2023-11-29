#include "DelayManager.h"

DelayManager::DelayManager() {
	methodDelays.fill(Nanoseconds(0));
	//DisplayInfoBox(L"DelayManager", L"Construction success");
}

Nanoseconds DelayManager::getDelay(int methodIndex){
	return methodDelays[methodIndex];
}

void DelayManager::updateDelays(delayArray _newDelays){
	methodDelays = _newDelays;
	for (int i = 0; i < D3D11_METHOD_COUNT; i++) {
		//DisplayInfoBox(L"Delay added for method " + std::to_wstring(i), std::to_wstring(methodDelays[i].count()));
	}
}

void DelayManager::resetDelays() {
	methodDelays.fill(Nanoseconds(0));
}