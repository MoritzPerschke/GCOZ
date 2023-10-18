#pragma once
#include <Windows.h>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <array>
#include <algorithm>
#include <string>
#include <vector>

using threadID = std::thread::id;

template <typename T> int getIndex(std::vector<T> _vec, T _elem) {
	auto it = std::find(_vec.begin(), _vec.end(), _elem);
	int idx = 0;
	it != _vec.end() ? idx = static_cast<int>(it - _vec.begin()) : idx = -1;
	return idx;
}

namespace ThreadIDs {

	static std::mutex mutex;
	static std::vector<threadID> knownThreads;
	static std::unordered_map<int, std::vector<idHash>> threadMethods;

	void addID(int _methodIdx, threadID _id) {
		std::size_t hash = std::hash<std::thread::id>{}(_id);
		/*
		* if threadIdx is -1 this thread has never called a method before
		* if it has, check if the map at key method already knows the thread
		* it it does, don't do anything, else add it
		*/
		if (std::find(knownThreads.begin(), knownThreads.end(), _id) != knownThreads.end()){
			int threadIdx = getIndex(knownThreads, _id) + 1;
			if (std::find(threadMethods[_methodIdx].begin(), threadMethods[_methodIdx].end(), hash) == threadMethods[_methodIdx].end()){
				threadMethods[_methodIdx].push_back(hash); // does this work as intended??
			} // if thread has not called method before
		} // if thread is known
		else {
			knownThreads.push_back(_id);
			int threadIdx = getIndex(knownThreads, _id) + 1;
			threadMethods[_methodIdx].push_back(threadIdx);
		} // new thread calling a method, assumption made threadMethods[_methodIdx] doesn't contain it
	}

	idArray getIDs(int _method) {
		idArray result = {};
		if (threadMethods[_method].size() > 0) { // this clause never catches for some reason
		}
		for (int i = 0; i < threadMethods[_method].size(); i++) {
			result[i] = threadMethods[_method][i];
		}
		return result;
	}
}