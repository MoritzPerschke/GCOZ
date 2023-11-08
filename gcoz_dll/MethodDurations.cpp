#include "MethodDurations.h"

Timepoint MethodDurations::now() {
	return clock.now();
}

MethodDurations::MethodDurations() {
	managed_shared_memory segment(open_only, "gcoz_SharedMemory");
	_durations = segment.find<IPC::DurationVector_Map>("Durations_Map").first;
	_frameRates = segment.find<IPC::Results_Map>("FrameRates_Map").first;
	_frameTimes = segment.find<IPC::Results_Map>("FrameTimes_Map").first;

}

void MethodDurations::addDuration(int _methodIdx, RawDuration _dur) {
	named_mutex durationMutex(open_only, "gcoz_Durations_Map_Mutex");
	scoped_lock<named_mutex> durationLock(durationMutex);
	_durations->at(_methodIdx).push_back(duration_cast<Nanoseconds>(_dur).count());
}

void MethodDurations::presentStart(int _method, float _delay) {
	using std::chrono::duration_cast;
	Timepoint currentPresentStart = now();
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
