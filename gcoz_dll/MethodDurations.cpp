#include "MethodDurations.h"

Timepoint MethodDurations::now() {
	return clock.now();
}

void MethodDurations::addDuration(int _methodIdx, RawDuration _dur) {

	if(duration_cast<Nanoseconds>(_dur).count() > 100){
		auto it = _durations->find(_methodIdx);
		if (it == _durations->end()) {
			it = _durations->emplace(_methodIdx, _durations->get_allocator()).first;
		}
		it->second.push_back(duration_cast<Nanoseconds>(_dur));
	}
}

void MethodDurations::presentStart(int _method, int _delay, int &_counter) {
	using std::chrono::duration_cast;
	Timepoint currentPresentStart = now();
	if (!presentCalledInit){
		_counter--; 
	}

	else if(_counter < MEASURE_FRAME_COUNT) {
		// get iterators to the map for the method and for the map for the delay
		named_mutex timesMutex(open_only, "gcoz_FrameTimes_Map_Mutex");
		scoped_lock<named_mutex> timesLock(timesMutex);
		frameTimes[_method][_delay].push_back(duration_cast<Nanoseconds> (currentPresentStart - lastPresentEnd));
	}
	lastPresentStart = currentPresentStart;
}

void MethodDurations::presentEnd() {
	lastPresentEnd = now();
	if (!presentCalledInit) { presentCalledInit = true; }
}

void MethodDurations::reset() {
	presentCalledInit = false;
}

void MethodDurations::finish(int _method, int _delay) {
	named_mutex timesMutex(open_only, "gcoz_FrameTimes_Map_Mutex");
	scoped_lock<named_mutex> timesLock(timesMutex);

	auto method_it = _frameTimes->emplace(_method, _frameTimes->get_allocator()).first;
	auto delay_it = method_it->second.emplace(_delay, method_it->second.get_allocator()).first;

	for (const auto& time : frameTimes[_method][_delay]) {
		delay_it->second.push_back(time);
	}
}