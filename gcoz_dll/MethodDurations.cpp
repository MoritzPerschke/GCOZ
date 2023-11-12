#include "MethodDurations.h"

Timepoint MethodDurations::now() {
	return clock.now();
}

MethodDurations::MethodDurations() {
	managed_shared_memory segment(open_only, "gcoz_SharedMemory");
	_durations = segment.find<IPC::DurationVector_Map>("Durations_Map").first;
	_frameRates = segment.find<IPC::Results_Map>("FrameRates_Map").first;
	_frameTimes = segment.find<IPC::Results_Map>("FrameTimes_Map").first;
	//DisplayInfoBox(L"MethodDurations", L"Construction success");
	if (_frameRates == NULL || _frameTimes == NULL) {
		DisplayErrorBox(L"MethodDurations");
	}

}

void MethodDurations::addDuration(int _methodIdx, RawDuration _dur) {
	managed_shared_memory segment(open_only, "gcoz_SharedMemory");

	named_mutex durationMutex(open_only, "gcoz_Durations_Map_Mutex");
	scoped_lock<named_mutex> durationLock(durationMutex);

	// not sure why but map needs to be reopened every time, can't do this in the constructor
	// this obviously ruins fps, do writing in finish() method or similar
	IPC::DurationVector_Map* _durations = segment.find<IPC::DurationVector_Map>("Durations_Map").first; 
	// check if shared map contains element at _methodIdx
	if (_durations->size() == 0 || _durations->find(_methodIdx) == _durations->end()) {
		// if no create
		IPC::Duration_Allocator alloc = segment.get_segment_manager();
		_durations->insert(IPC::DurationMap_Type(_methodIdx, IPC::Duration_Vector(alloc))); // now this doesn't work??
	}

	// insert duration
	_durations->at(_methodIdx).push_back(duration_cast<Nanoseconds>(_dur).count());
}

void MethodDurations::presentStart(int _method, float _delay) {
	using std::chrono::duration_cast;
	Timepoint currentPresentStart = now();

	// check if framerates map contains element at _delay at method
	if (_frameRates->find(_method) == _frameRates->end())
	{ // method has not been delayed at all so far
		managed_shared_memory segment(open_only, "gcoz_SharedMemory");
		IPC::DelayMapValue_Allocator alloc = segment.get_segment_manager();
		
		// insert map for method
		_frameRates->insert(IPC::ResultsMap_Type(_method, IPC::DelayResult_Map(alloc)));

		for (int i = 0; i < 10; i++)
		{ //create map elements for all delays
			float delay = static_cast<float>(i) / 10;
			IPC::Duration_Allocator durAlloc = segment.get_segment_manager();
			_frameRates->at(_method).insert(IPC::DelayMap_Type(delay, IPC::Duration_Vector(durAlloc)));
		}
	}
	// repeat for frame times
	if (_frameTimes->find(_method) == _frameTimes->end())
	{ // method has not been delayed at all so far
		managed_shared_memory segment(open_only, "gcoz_SharedMemory");
		IPC::DelayMapValue_Allocator alloc = segment.get_segment_manager();

		// insert map for method
		_frameTimes->insert(IPC::ResultsMap_Type(_method, IPC::DelayResult_Map(alloc)));

		for (int i = 0; i < 10; i++)
		{ //create map elements for all delays
			float delay = static_cast<float>(i) / 10;
			IPC::Duration_Allocator durAlloc = segment.get_segment_manager();
			_frameTimes->at(_method).insert(IPC::DelayMap_Type(delay, IPC::Duration_Vector(durAlloc)));
		}
	}

	if (_frameRates->size() < MEASURE_FRAME_COUNT && _frameTimes->size() < MEASURE_FRAME_COUNT) {

		Nanoseconds rate = duration_cast<Nanoseconds> (currentPresentStart - lastPresentStart);
		Nanoseconds time = duration_cast<Nanoseconds> (currentPresentStart - lastPresentEnd);

		named_mutex ratesMutex(open_only, "gcoz_Framerates_Map_Mutex");
		scoped_lock<named_mutex> ratesLock(ratesMutex);
		_frameRates->at(_method).at(_delay).push_back(rate.count());

		named_mutex timesMutex(open_only, "gcoz_FrameTimes_Map_Mutex");
		scoped_lock<named_mutex> timesLock(timesMutex);
		_frameTimes->at(_method).at(_delay).push_back(time.count());

	}
	lastPresentStart = currentPresentStart;
}

void MethodDurations::presentEnd() {
	lastPresentEnd = now();
}
