#pragma once
#include <Windows.h>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <array>
#include <algorithm>
#include <string>
#include <vector>
#include <SharedMemoryBoost.hpp>

#include "ErrorMessage.h"

template <typename T> int getIndex(std::vector<T> _vec, T _elem) {
	auto it = std::find(_vec.begin(), _vec.end(), _elem);
	int idx = 0;
	it != _vec.end() ? idx = static_cast<int>(it - _vec.begin()) : idx = -1;
	return idx;
}

class ThreadIDs {

	//std::vector<threadID> _knownThreads;
	managed_shared_memory segment;
	IPC::ThreadIdVector_Map* _threadIDMap;

public:
	std::mutex mutex;
	ThreadIDs() :
		segment{ open_only, "gcoz_SharedMemory" },
		_threadIDMap(segment.find<IPC::ThreadIdVector_Map>("ThreadID_Map").first) {}

	void addID(int _methodIdx, threadID _id);
};