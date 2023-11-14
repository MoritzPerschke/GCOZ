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
	
	managed_shared_memory segment;
	IPC::DurationVector_Map* _durations;
	IPC::ResultsMap_Map* _frameTimes;
	IPC::ResultsMap_Map* _frameRates;

public:
	MethodDurations() :
		segment{ open_only, "gcoz_SharedMemory" },
		_durations(segment.find<IPC::DurationVector_Map>("Durations_Map").first),
		_frameTimes(segment.find<IPC::ResultsMap_Map>("FrameTimes_Map").first),
		_frameRates(segment.find<IPC::ResultsMap_Map>("FrameRates_Map").first) {}

	Timepoint now();
	void addDuration(int _methodIdx, RawDuration _duration);
	void presentStart(int _method, float _delay);
	void presentEnd();

};
