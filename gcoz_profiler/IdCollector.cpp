#include "IdCollector.h"

IdCollector::IdCollector() {
	this->currentMethod = 0;
	this->lastMethod = 0;
}

void IdCollector::addIDs(idArray _threadIDs) {
	for (const auto& id : _threadIDs) {
		if (id != 0) {
			std::cout << inf << "hashed threadid: " << id << std::endl;
			this->ids[currentMethod].push_back(id);
		}
	}
	if (this->ids[currentMethod].size() == 0) {
		//std::cout << inf << "No ThreadIDs received for " << currentMethod << std::endl;
	}
	currentMethod++;
}

int IdCollector::nextMethod() {
	std::cout << "[IdCollector] current Method:" << methodNames[currentMethod] << std::endl;
	return currentMethod;
}

bool IdCollector::isDone() {
	return currentMethod == D3D11_METHOD_COUNT;
}

void IdCollector::finish(ResultsHandler& resH) {
	resH.addThreadIDs(this->ids);
}