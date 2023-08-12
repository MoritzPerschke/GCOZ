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
	static int presentCalls = 0;
	static std::array<int, D3D11_METHOD_COUNT> calls = {};
	static std::array<Nanoseconds, D3D11_METHOD_COUNT> durations;

	static Timepoint lastPresentCall;
	static std::array<Nanoseconds, MEASURE_FRAME_COUNT> presentCallTimes;
	static std::chrono::steady_clock clock; // https://stackoverflow.com/a/37440647/15005309

	Timepoint now() {
		return clock.now();
	}

	void addDuration(int _methodIdx, Duration _duration) {
		calls[_methodIdx]++;
		durations[_methodIdx] += std::chrono::duration_cast<Nanoseconds>(_duration); // not here
	}

	void presentCalled() {
		static bool init = false;
		static int callNr = 0;
		if (!init) {
			lastPresentCall = now();
			init = true;
		}
		else {
			if (presentCalls < MEASURE_FRAME_COUNT) {
				presentCallTimes[presentCalls] = std::chrono::duration_cast<Nanoseconds>(now() - lastPresentCall);
				presentCalls++;
			} // not here
			lastPresentCall = now();
		}
	}

	std::array<Nanoseconds, MEASURE_FRAME_COUNT> getPresentTimes() {
		presentCalls = 0;
		return presentCallTimes;
	}

	std::array<std::chrono::nanoseconds, D3D11_METHOD_COUNT> getDurations() {
		std::array<std::chrono::nanoseconds, D3D11_METHOD_COUNT> returnDurations = durations;
		for (int i = 0; i < D3D11_METHOD_COUNT; i++) {
			if (calls[i] != 0) {
				_msg.durations[i] = _msg.durations[i] / calls[i];
			}
			else {
				Timepoint empty = now();
				_msg.durations[i] = empty - empty;
			}
		}
	}
} // MethodDurations