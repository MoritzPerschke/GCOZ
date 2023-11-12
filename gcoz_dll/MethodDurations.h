#pragma once
#include <chrono>
#include <array>
#include <algorithm>
#include <vector>
#include <SharedMemoryBoost.hpp>
#include <Constants.h>

#include "ErrorMessage.h"

class MethodDurations {
	Timepoint lastPresentStart;
	Timepoint lastPresentEnd;

	static std::chrono::steady_clock clock;

	IPC::DurationVector_Map* _durations;
	IPC::Results_Map* _frameTimes;
	IPC::Results_Map* _frameRates;

public:
	MethodDurations();
	Timepoint now();
	void addDuration(int _methodIdx, RawDuration _duration);
	void presentStart(int _method, float _delay);
	void presentEnd();

};
