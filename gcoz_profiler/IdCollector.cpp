#include "IdCollector.h"

IdCollector::IdCollector() {
	this->currentMethod = 0;
}

void IdCollector::idsAdded() {
	//currentMethod++;
}

int IdCollector::nextMethod() {
	currentMethod = calledMethods.front();
	calledMethods.pop();
	spdlog::info("Current method: {}", methodNames[currentMethod]);
	return currentMethod;
}

bool IdCollector::isDone() {
	return calledMethods.size() == 0;
}

std::queue<int>& IdCollector::getRefMethodQueue()
{
	return calledMethods;
}
