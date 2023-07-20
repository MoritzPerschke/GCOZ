#pragma once
#include <chrono>
#include <array>
#include <algorithm>

namespace MethodDurations {
	using Timepoint = std::chrono::steady_clock::time_point;
	using Duration = std::chrono::duration<double>;

	// maybe use map here to keep track of all individual times
	static std::array<int, 205> calls = {}; // does not compile
	static std::array<std::chrono::nanoseconds, 205> durations;
	static std::chrono::steady_clock clock; // https://stackoverflow.com/a/37440647/15005309

	Timepoint now() {
		return clock.now();
	}

	void addDuration(int methodIdx, Duration _duration) {
		calls[methodIdx]++;
		durations[methodIdx] += std::chrono::duration_cast<std::chrono::nanoseconds>(_duration);
	}

	std::array<std::chrono::nanoseconds, 205> getDurations() {
		std::array<std::chrono::nanoseconds, 205> returnDurations = durations;
		for (int i = 0; i < 205; i++) {
			returnDurations[i] = returnDurations[i] / calls[i];
		}
		return returnDurations;
	}
} // MethodDurations