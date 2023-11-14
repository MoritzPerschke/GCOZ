#include "MethodDurations.h"

Timepoint MethodDurations::now() {
	return clock.now();
}

//MethodDurations::MethodDurations() {
//	managed_shared_memory segment(open_only, "gcoz_SharedMemory");
//	_durations = segment.find<IPC::DurationVector_Map>("Durations_Map").first;
//	_frameRates = segment.find<IPC::Results_Map>("FrameRates_Map").first;
//	_frameTimes = segment.find<IPC::Results_Map>("FrameTimes_Map").first;
//	//DisplayInfoBox(L"MethodDurations", L"Construction success");
//	if (_frameRates == NULL || _frameTimes == NULL) {
//		DisplayErrorBox(L"MethodDurations");
//	}
//
//}

void MethodDurations::addDuration(int _methodIdx, RawDuration _dur) {

	auto it = _durations->find(_methodIdx);
	if (it == _durations->end()) {
		it = _durations->emplace(_methodIdx, _durations->get_allocator()).first;
		it->second.push_back(duration_cast<Nanoseconds>(_dur).count());
	}
	else {
		it->second.push_back(duration_cast<Nanoseconds>(_dur).count());
	}
}

void MethodDurations::presentStart(int _method, float _delay) {
	using std::chrono::duration_cast;
	Timepoint currentPresentStart = now();

	if (_frameRates->size() < MEASURE_FRAME_COUNT && _frameTimes->size() < MEASURE_FRAME_COUNT) {
		/* get iterators to the map for the method and for the map for the delay */
		auto rates_method_it = _frameRates->find(_method);
		if (rates_method_it == _frameRates->end()) {
			rates_method_it = _frameRates->emplace(_method, _frameRates->get_allocator()).first; // insert map for method results and assign iterator to new map
			for (int i = 0; i < 10; i++) {
				float delay = static_cast<float>(i) / 10;
				rates_method_it->second.emplace(delay, rates_method_it->second.get_allocator()); // insert map for delay results
			} // not sure doing all at once is best
		}

		auto rates_delay_it = rates_method_it->second.find(_delay);
		if (rates_delay_it == rates_method_it->second.end()) { // this should not catch, see above
			rates_delay_it = rates_method_it->second.emplace(_delay, rates_method_it->second.get_allocator()).first;
		}

		auto times_method_it = _frameTimes->find(_method);
		if (times_method_it == _frameTimes->end()) {
			times_method_it = _frameTimes->emplace(_method, _frameTimes->get_allocator()).first;
			for (int i = 0; i < 10; i++) {
				float delay = static_cast<float>(i) / 10;
				times_method_it->second.emplace(delay, times_method_it->second.get_allocator());
			}
		}

		auto times_delay_it = times_method_it->second.find(_delay);
		if (times_delay_it == times_method_it->second.end()) {
			times_delay_it = times_method_it->second.emplace(_delay, times_method_it->second.get_allocator()).first;
		}


		Nanoseconds rate = duration_cast<Nanoseconds> (currentPresentStart - lastPresentStart);
		Nanoseconds time = duration_cast<Nanoseconds> (currentPresentStart - lastPresentEnd);

		named_mutex ratesMutex(open_only, "gcoz_Framerates_Map_Mutex");
		scoped_lock<named_mutex> ratesLock(ratesMutex);
		rates_delay_it->second.push_back(rate.count());

		named_mutex timesMutex(open_only, "gcoz_FrameTimes_Map_Mutex");
		scoped_lock<named_mutex> timesLock(timesMutex);
		times_delay_it->second.push_back(time.count());
	}
	lastPresentStart = currentPresentStart;
}

void MethodDurations::presentEnd() {
	lastPresentEnd = now();
}
