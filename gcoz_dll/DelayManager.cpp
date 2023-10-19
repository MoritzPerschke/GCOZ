#include "DelayManager.h"

DelayManager::DelayManager() {
	methodDelays.fill(Nanoseconds(0));
}

Nanoseconds DelayManager::getDelay(int methodIndex){
	return methodDelays[methodIndex];
}

void DelayManager::updateDelays(delayArray _newDelays){
	methodDelays = _newDelays;
}

void DelayManager::resetDelays() {
	methodDelays.fill(Nanoseconds(0));
}