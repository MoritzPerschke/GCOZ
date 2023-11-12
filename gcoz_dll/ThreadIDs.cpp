#include "ThreadIDs.h"

ThreadIDs::ThreadIDs(){
	HANDLE beginEvent = OpenEvent(NULL, NULL, L"gcoz_SharedMemory_ThreadIDs_created");
	if (beginEvent != NULL) {
		WaitForSingleObject(beginEvent, INFINITE); // make sure shared memory is created
	}

	managed_shared_memory segment(open_only, "gcoz_SharedMemory");
	_threadIDMap = segment.find<IPC::ThreadIdVector_Map>("gcoz_SharedMemory_ThreadIDs").first;
	//DisplayInfoBox(L"ThreadIDs", L"Construction success");
}

void ThreadIDs::addID(int _methodIdx, threadID _id) {
	using std::find;
	
	std::size_t hash = std::hash<std::thread::id>{}(_id);
	named_mutex mutex(open_only, "gcoz_SharedMemory_generalMutex");

	// check if shared map contains element at _methodIdx
	if (_threadIDMap->find(_methodIdx) == _threadIDMap->end()) {
		
		// if no create
		managed_shared_memory segment(open_only, "gcoz_SharedMemory");
		IPC::IdHash_Allocator alloc = segment.get_segment_manager();

		scoped_lock<named_mutex> lock(mutex);
		_threadIDMap->insert(IPC::IdHashMap_Type(_methodIdx, IPC::IdHash_Vector(alloc)));
	}

	// _thread has never called a method before
	if (find(_knownThreads.begin(), _knownThreads.end(), _id) == _knownThreads.end()) {
		_knownThreads.push_back(_id);

		scoped_lock<named_mutex> lock(mutex);
		_threadIDMap->at(_methodIdx).push_back(hash);
	}
	else {
		// method has never been called by thread -> hash not in vector at _methodIdx
		if (find(_threadIDMap->at(_methodIdx).begin(), _threadIDMap->at(_methodIdx).end(), hash) == _threadIDMap->at(_methodIdx).end()) {

			scoped_lock<named_mutex> lock(mutex);
			_threadIDMap->at(_methodIdx).push_back(hash);
		}
	}
}