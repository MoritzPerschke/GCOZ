#include "IdCollector.h"

IdCollector::IdCollector() {
	this->lastMethod = -1;
	std::cout << err << "[IDCollector] Constructor called without arguments" << std::endl;
}

// never gets called?
IdCollector::IdCollector(ResultsHandler& _handler) {
	std::cout << ok << "[IDCollector] Constructor called properly" << std::endl;
	this->lastMethod = -1;
	this->handler = _handler;
}

void IdCollector::addIDs(std::array<long long, METHOD_ID_ARRAY_SIZE> _threadIDs) {
	for (const auto& id : _threadIDs) {
		this->ids[lastMethod].push_back(id);
	}
}

int IdCollector::nextMethod() {
	std::cout << "[IdCollector] nextMethod() called, lastMethod:" << lastMethod << std::endl;
	return ++lastMethod;
}

bool IdCollector::isDone() {
	return lastMethod == D3D11_METHOD_COUNT;
}

void IdCollector::finish() {
	this->handler.addThreadIDs(this->ids);
}