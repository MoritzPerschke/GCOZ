#pragma once
#include <chrono>
#include <array>
#include <algorithm>
#include <vector>

namespace MethodDurations {
	using Timepoint = std::chrono::steady_clock::time_point;
	using Duration = std::chrono::duration<double>;
	using Nanoseconds = std::chrono::nanoseconds;

	// maybe use map here to keep track of all individual times
	static std::array<int, 205> calls = {};
	static std::array<Nanoseconds, 205> durations;

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
			DisplayErrorBox(L"getPresentTimes()", L"presentCallTimes is empty");
			return 0;
		}

		for (const auto& elem : presentCallTimes) {
			_msg.frameTimepoints.push_back(elem);
		}
		return _msg.frameTimepoints.size();
	}

	std::array<std::chrono::nanoseconds, 205> getDurations() {
		std::array<std::chrono::nanoseconds, 205> returnDurations = durations;
		for (int i = 0; i < 205; i++) {
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