#include "MethodDurations.h"

Timepoint MethodDurations::now() {
	return clock.now();
}

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

void MethodDurations::presentStart(int _method, int _delay) {
	using std::chrono::duration_cast;
	Timepoint currentPresentStart = now();

	if (_frameRates->size() < MEASURE_FRAME_COUNT && _frameTimes->size() < MEASURE_FRAME_COUNT) {
		/* get iterators to the map for the method and for the map for the delay */
		auto rates_method_it = _frameRates->find(_method);
		if (rates_method_it == _frameRates->end()) {
			rates_method_it = _frameRates->emplace(_method, _frameRates->get_allocator()).first; // insert map for method results and assign iterator to new map
		}
		auto rates_delay_it = rates_method_it->second.find(_delay);
		if (rates_delay_it == rates_method_it->second.end()) { // this should not catch, see above
			rates_delay_it = rates_method_it->second.emplace(_delay, rates_method_it->second.get_allocator()).first;
		}

		auto times_method_it = _frameTimes->find(_method);
		if (times_method_it == _frameTimes->end()) {
			times_method_it = _frameTimes->emplace(_method, _frameTimes->get_allocator()).first;
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
		//DisplayInfoBox(L"MethodDurations.cpp", L"Added FrameRate at delay: " + std::to_wstring(rates_delay_it->first) + L" for method: " + std::to_wstring(rates_method_it->first));

		named_mutex timesMutex(open_only, "gcoz_FrameTimes_Map_Mutex");
		scoped_lock<named_mutex> timesLock(timesMutex);
		times_delay_it->second.push_back(time.count());
	}
	lastPresentStart = currentPresentStart;
}

void MethodDurations::presentEnd() {
	lastPresentEnd = now();
}
