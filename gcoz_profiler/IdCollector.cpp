#include "IdCollector.h"

IdCollector::IdCollector() {
	throw std::runtime_error("IdCollector Contructor called without arguments");
}

IdCollector::IdCollector(ResultsHandler& _handler) {
	lastMethod = 0;
	this->handler = _handler;
}

void IdCollector::addIDs(std::array<long long, METHOD_ID_ARRAY_SIZE> _threadIDs) {
	std::cout << inf << "Added Thread Id's for method" << methodNames[lastMethod] << "(" << lastMethod << ")" << std::endl;
	for (const auto& elem : _threadIDs) {
		if (elem != 0) {
			ids[lastMethod].push_back(elem);
		}
	}
}

int IdCollector::nextMethod() {
	return ++lastMethod;
}

bool IdCollector::isDone() {
	return lastMethod == D3D11_METHOD_COUNT;
}

void IdCollector::finish() {
	this->handler.addThreadIDs(ids);
}