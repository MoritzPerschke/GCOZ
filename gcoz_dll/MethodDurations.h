#pragma once
#include <chrono>
#include <array>
#include <algorithm>
#include <vector>

#include "../shared/Constants.h"

namespace MethodDurations {
	using Timepoint = std::chrono::steady_clock::time_point;
	using Duration = std::chrono::duration<double>;
	//using Nanoseconds = std::chrono::microseconds; // when this line is active, results look good?

	// maybe use map here to keep track of all individual times
	static int presentCalls = 0;
	static bool presentCalledInit = false;
	static std::array<int, D3D11_METHOD_COUNT> calls = {};
	static durationArray durations;

	static Timepoint lastPresentEnd;
	static Timepoint lastPresentStart;
	frametimeArray presentCallTimes;
	frametimeArray frameRates;
	static std::chrono::steady_clock clock; // https://stackoverflow.com/a/37440647/15005309

	Timepoint now() {
		return clock.now();
	}

	void addDuration(int _methodIdx, Duration _duration) {
		calls[_methodIdx]++;
		durations[_methodIdx] += std::chrono::duration_cast<Nanoseconds>(_duration); // not here
	}

	void presentStart() {
		if (presentCalledInit) {
			if (presentCalls < MEASURE_FRAME_COUNT) {
				frameRates[presentCalls] = std::chrono::duration_cast<Nanoseconds>(now() - lastPresentStart);
				lastPresentStart = now();

				presentCallTimes[presentCalls] = std::chrono::duration_cast<Nanoseconds>(now() - lastPresentEnd);
				presentCalls++;
			}
		}
	}

	void presentEnd() {
		lastPresentEnd = now();
		if (!presentCalledInit) { presentCalledInit = true; }
	}

	frametimeArray getFrameRates() {
		return frameRates;
	}

	std::array<int, D3D11_METHOD_COUNT> getCallAmounts() {
		return calls;
	}

	frametimeArray getPresentTimes() {
		presentCalls = 0;
		presentCalledInit = false;
		return presentCallTimes;
	}

	durationArray getDurations() {
		durationArray returnDurations = durations;
		for (int i = 0; i < D3D11_METHOD_COUNT; i++) {
			if(calls[i] == 0){
				Timepoint empty = now();
				returnDurations[i] = std::chrono::duration_cast<Nanoseconds>(empty - empty);
			}
		}
		return returnDurations;
	}
} // MethodDurations