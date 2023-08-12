#pragma once
#include <chrono>
#include <array>
#include <algorithm>
#include <vector>

#include "../gcoz_profiler/Constants.h"

namespace MethodDurations {
	using Timepoint = std::chrono::steady_clock::time_point;
	using Duration = std::chrono::duration<double>;
	using Nanoseconds = std::chrono::nanoseconds;

	// maybe use map here to keep track of all individual times
	static std::array<int, D3D11_METHOD_COUNT> calls = {};
	static std::array<Nanoseconds, D3D11_METHOD_COUNT> durations;

	static Timepoint lastPresentCall;
	static std::vector<Nanoseconds> presentCallTimes;
	static std::chrono::steady_clock clock; // https://stackoverflow.com/a/37440647/15005309

	Timepoint now() {
		return clock.now();
	}

	void addDuration(int methodIdx, Duration _duration) {
		calls[methodIdx]++;
		durations[methodIdx] += std::chrono::duration_cast<Nanoseconds>(_duration);
	}

	void presentCalled() {
		static bool init = false;
		if (!init) {
			lastPresentCall = now();
			init = true;
		}
		else {
			presentCallTimes.push_back(std::chrono::duration_cast<Nanoseconds>(now() - lastPresentCall));
			lastPresentCall = now();
		}
	}

	size_t getPresentTimes(DllMessage _msg) {
		if (presentCallTimes.size() == 0) {
			//DisplayErrorBox(L"getPresentTimes()", L"presentCallTimes is empty");
			return 0;
		}

		for (const auto& elem : presentCallTimes) {
			_msg.frameTimepoints.push_back(elem);
		}
		return _msg.frameTimepoints.size();
	}

	std::array<std::chrono::nanoseconds, D3D11_METHOD_COUNT> getDurations() {
		std::array<std::chrono::nanoseconds, D3D11_METHOD_COUNT> returnDurations = durations;
		for (int i = 0; i < D3D11_METHOD_COUNT; i++) {
			if (calls[i] != 0) {
				returnDurations[i] = returnDurations[i] / calls[i];
			}
			else {
				Timepoint empty = now();
				returnDurations[i] = empty - empty;
			}
		}
		return returnDurations;
	}
} // MethodDurations