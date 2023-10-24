#include "IdCollector.h"

IdCollector::IdCollector() {
	this->currentMethod = 0;
	this->lastMethod = 0;
}

void IdCollector::addIDs(idArray _threadIDs) {
	for (const auto& id : _threadIDs) {
		if (id != 0) {
			spdlog::info("Hashed threadid for {}: {}", methodNames[currentMethod], id);
			this->ids[currentMethod].push_back(id);
		}
	}
	if (this->ids[currentMethod].size() == 0) {
		spdlog::warn("No ThreadIDs received for {}", methodNames[currentMethod]);
	}
	currentMethod++;
}

int IdCollector::nextMethod() {
	spdlog::info("Current method: {}", methodNames[currentMethod]);
	return currentMethod;
}

bool IdCollector::isDone() {
	return currentMethod == D3D11_METHOD_COUNT;
}

void IdCollector::finish(ResultsHandler& resH) {
	resH.addThreadIDs(this->ids);
}