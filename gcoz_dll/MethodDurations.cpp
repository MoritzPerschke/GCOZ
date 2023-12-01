#include "MethodDurations.h"

Timepoint MethodDurations::now() {
	return clock.now();
}

void MethodDurations::addDuration(int _methodIdx, RawDuration _dur) {

	if(duration_cast<Nanoseconds>(_dur).count() > 0){
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
		named_mutex timesMutex(open_only, "gcoz_FrameTimes_Map_Mutex");
		scoped_lock<named_mutex> timesLock(timesMutex);
		frameTimes[_method][_delay].push_back(duration_cast<Nanoseconds> (currentPresentStart - lastPresentEnd));
		///* get iterators to the map for the method and for the map for the delay */
		//auto rates_method_it = _frameRates->find(_method);
		//if (rates_method_it == _frameRates->end()) {
		//	rates_method_it = _frameRates->emplace(_method, _frameRates->get_allocator()).first; // insert map for method results and assign iterator to new map
		//}
		//
		//auto rates_delay_it = rates_method_it->second.find(_delay);
		//if (rates_delay_it == rates_method_it->second.end()) {
		//	rates_delay_it = rates_method_it->second.emplace(_delay, rates_method_it->second.get_allocator()).first;
		//}

		//Nanoseconds rate = duration_cast<Nanoseconds> (currentPresentStart - lastPresentStart);
		//named_mutex ratesMutex(open_only, "gcoz_Framerates_Map_Mutex");
		//scoped_lock<named_mutex> ratesLock(ratesMutex);
		//rates_delay_it->second.push_back(rate);

		//auto times_method_it = _frameTimes->find(_method);
		//if (times_method_it == _frameTimes->end()) {
		//	times_method_it = _frameTimes->emplace(_method, _frameTimes->get_allocator()).first;
		//}

		//auto times_delay_it = times_method_it->second.find(_delay);
		//if (times_delay_it == times_method_it->second.end()) {
		//	times_delay_it = times_method_it->second.emplace(_delay, times_method_it->second.get_allocator()).first;
		//}

		//Nanoseconds time = duration_cast<Nanoseconds> (currentPresentStart - lastPresentEnd);
		//named_mutex timesMutex(open_only, "gcoz_FrameTimes_Map_Mutex");
		//scoped_lock<named_mutex> timesLock(timesMutex);
		//times_delay_it->second.push_back(time);
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

	//DisplayInfoBox(L"Length of map<<>>", std::to_wstring(_frameTimes->size()));
	//for (const auto& method : frameTimes) {
	//	auto method_it = _frameTimes->emplace(method.first, _frameTimes->get_allocator()).first;
	//	DisplayInfoBox(L"Length of map<" + std::to_wstring(method.first) + L", vec>: ", std::to_wstring(frameTimes[method.first].size()));
	//	for (const auto& delay : method.second) {
	//		auto delay_it = method_it->second.emplace(delay.first, method_it->second.get_allocator()).first;
	//		DisplayInfoBox(L"Length of vec: ", std::to_wstring(frameTimes[method.first][delay.first].size()));
	//	}
	//}
}