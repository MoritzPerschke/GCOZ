#pragma once
#include <Windows.h>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <array>
#include <algorithm>
#include <string>

//delete
#include <vector>
#include <map>

using threadID = std::thread::id;

int getIndex(std::vector<threadID> v, threadID e) {
	auto it = std::find(v.begin(), v.end(), e);
	int idx = 0;
	it != v.end() ? idx = static_cast<int>(it - v.begin()) : idx = -1;
	return idx;
}

/// ?associate method with thread or thread with method?
namespace ThreadIDs {

	std::mutex mutex;
	std::unordered_map<threadID, std::vector<int>> threadMethods;

	//void addID(threadID _id, int _methodIdx) {
	void addID(int _methodIdx) {
		std::lock_guard<std::mutex> guard(mutex);
		threadMethods[std::this_thread::get_id()].push_back(_methodIdx); // does this work as intended??
	}

	/* 
	  iterate map<methodIdx, threadsIDs>, iterate threadIDs vector; add index of threadID in temp vector to return map[methodIdx]
	*/
	std::array<long long, METHOD_ID_ARRAY_SIZE> getIDs(int _method) {
		std::map<int, std::vector<int>> threadINT;
		std::vector<threadID> temp;

		for (const auto& it : threadMethods) {
			// first  == threadID
			// second == vector of methods called by thread
			if (std::find(temp.begin(), temp.end(), it.first) != temp.end()) {
				temp.push_back(it.first);
			}
			std::copy(it.second.begin(), it.second.end(), threadMethods[it.first].begin());
		}

		std::array<long long, METHOD_ID_ARRAY_SIZE> result = { -1 };
		for (int i = 0; i < threadINT[_method].size(); i++) {
			result[i] = threadINT[_method][i];
		}
		///// TEMP: build string of integer thread ids to show in msg box
		//MessageBoxW(NULL, L"Starting build of ID Messageboxes", L"Debug", MB_SETFOREGROUND);
		//for (const auto& it : threadINT) {
		//	// first  == id of thread
		//	// second == list of called methods
		//	if (it.second.size() > 0) {
		//		std::wstring bodyString = L"[";
		//		for (const auto& idx : it.second) {
		//			bodyString += std::to_wstring(idx) + L", ";
		//		}
		//		bodyString += L"] ";

		//		std::wstring titleString = L"Thread " + std::to_wstring(it.first) + L"called Methods";
		//		MessageBoxW(NULL, bodyString.c_str(), titleString.c_str(), MB_OK | MB_SETFOREGROUND);
		//	}
		//}
		return result;
	}
}