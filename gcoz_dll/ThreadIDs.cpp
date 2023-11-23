#include "ThreadIDs.h"

void ThreadIDs::addID(int _methodIdx, threadID _id) {
	using std::find;
	
	std::size_t hash = std::hash<std::thread::id>{}(_id);
	named_mutex mutex(open_only, "gcoz_ThreadID_Map_Mutex");

	// check if shared map contains element at _methodIdx
	auto it = _threadIDMap->find(_methodIdx);
	if (it == _threadIDMap->end()) {
		// if no create
		scoped_lock<named_mutex> lock(mutex);
		it = _threadIDMap->emplace(_methodIdx, _threadIDMap->get_allocator()).first;
	}

	if (find(it->second.begin(), it->second.end(), hash) == it->second.end()) {
		scoped_lock<named_mutex> lock(mutex);
		it->second.push_back(hash);
	}
}