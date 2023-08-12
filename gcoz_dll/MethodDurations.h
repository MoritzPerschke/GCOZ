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
	static std::array<Nanoseconds, MEASURE_FRAME_COUNT> presentCallTimes;
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
		static int callNr = 0;
		if (!init) {
			lastPresentCall = now();
			init = true;
		}
		else {
			presentCallTimes[callNr] = std::chrono::duration_cast<Nanoseconds>(now() - lastPresentCall);
			lastPresentCall = now();
		}
	}

	void getPresentTimes(DllMessage _msg) {
		_msg.frameTimepoints = presentCallTimes;
	}

	void getDurations(DllMessage _msg) {
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