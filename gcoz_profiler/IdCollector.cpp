#include "IdCollector.h"

IdCollector::IdCollector() {
	this->lastMethod = 0;
}

void IdCollector::addIDs(std::array<long long, METHOD_ID_ARRAY_SIZE> _threadIDs) {
	for (const auto& id : _threadIDs) {
		this->ids[lastMethod].push_back(id);
	}
}

int IdCollector::nextMethod() {
	std::cout << "[IdCollector] nextMethod() called, next method:" << lastMethod << std::endl;
	return lastMethod++;
}

bool IdCollector::isDone() {
	return lastMethod == D3D11_METHOD_COUNT;
}

void IdCollector::finish(ResultsHandler& resH) {
	resH.addThreadIDs(this->ids);
}