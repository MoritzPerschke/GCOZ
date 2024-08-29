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

/* A simple template that when instatiated returns the index of an element
*  if it exists in the given vector, -1 otherwise
*/
template <typename T> int getIndex(std::vector<T> _vec, T _elem) {
	auto it = std::find(_vec.begin(), _vec.end(), _elem);
	int idx = 0;
	it != _vec.end() ? idx = static_cast<int>(it - _vec.begin()) : idx = -1;
	return idx;
}

/* A simple object to track which D3D11 method calls which thread
*  Thread IDs are stored as the std::hash of a std::thread::id
*  in a map of vectors in shared memory
*/
class ThreadIDs {
	managed_shared_memory segment;
	IPC::ThreadIdVector_Map* _threadIDMap;

public:
	std::mutex mutex;
	// automatically instantiate the constructor as it is very simple
	ThreadIDs() :
		segment{ open_only, "gcoz_SharedMemory" },
		_threadIDMap(segment.find<IPC::ThreadIdVector_Map>("ThreadID_Map").first) {}

	void addID(int _methodIdx, threadID _id);
};