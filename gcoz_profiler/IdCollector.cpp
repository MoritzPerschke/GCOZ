#include "IdCollector.h"

IdCollector::IdCollector() {
	this->currentMethod = 0;
}

void IdCollector::idsAdded() {
	currentMethod++;
}

int IdCollector::nextMethod() {
	spdlog::info("Current method: {}", methodNames[currentMethod]);
	return currentMethod;
}

bool IdCollector::isDone() {
	return currentMethod == D3D11_METHOD_COUNT;
}
