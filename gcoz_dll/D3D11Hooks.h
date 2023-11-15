#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3d11_2.h>
#include <thread>

#include "ErrorMessage.h"
#include "HelperMacros.h"
#include "DelayManager.h"
#include "MethodDurations.h"
#include "ThreadIDs.h"
#include "DllGui.h"
#include "ProfilerStatusManager.h"
#include "../shared/Constants.h"

DelayManager delays;
MethodDurations durations;
ProfilerStatusManager man;
Communication com;
ThreadIDs ids;
std::string overlayDebugMessage = "Looking good :)"; // this is ugly, get this to a namespace or sth

void little_sleep(Nanoseconds _delay) { // https://stackoverflow.com/a/45571538
	auto start = durations.now();
	auto end = start + _delay;
	while (durations.now() < end) {
		if ((durations.now() - end) > std::chrono::milliseconds(1)) {
			std::this_thread::yield();
		}
	}
}

/* expands to function pointers for D3D11 methods */
#define X(idx, returnType, name, ...) typedef returnType (__stdcall* name)(PARAMETER_TYPES(__VA_ARGS__));
	D3D11_METHODS
	D3D11_METHODS_VOID
#undef X

/* expands to setting previously defined pointers to NULL */
#define X(idx, returnType, name, ...) static name o##name = NULL;
	D3D11_METHODS
	D3D11_METHODS_VOID
#undef X

/* expands to hooked d3d11 function */
// maybe always measure duration to verify pauses
#define X(_IDX, _RETURN_TYPE, _NAME, ...) \
	_RETURN_TYPE __stdcall hk##_NAME(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
		_RETURN_TYPE value; \
		Timepoint start; \
		switch (man.getStatus()) { \
			case ProfilerStatus::GCOZ_MEASURE : \
				start = durations.now(); \
				value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
				RawDuration duration = durations.now() - start; \
				durations.addDuration(_IDX, duration); \
				break; \
			case ProfilerStatus::GCOZ_PROFILE : \
				value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
				little_sleep(delays.getDelay(_IDX)); \
				break; \
			case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS: \
				value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
				ids.mutex.lock();\
				ids.addID(_IDX, std::this_thread::get_id());\
				ids.mutex.unlock(); \
			case ProfilerStatus::GCOZ_WAIT : \
				value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__));	\
				break; \
		} \
		return value; \
	}
	D3D11_METHODS
#undef X

/* same as above just for void functions (no return)*/
#define X(_IDX, _RETURN_TYPE, _NAME, ...) \
	_RETURN_TYPE __stdcall hk##_NAME(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
		Timepoint start; \
		switch (man.getStatus()) { \
			case ProfilerStatus::GCOZ_MEASURE : \
				start = durations.now(); \
				o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
				RawDuration duration = durations.now() - start; \
				durations.addDuration(_IDX, duration); \
				break; \
			case ProfilerStatus::GCOZ_PROFILE : \
				o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
				little_sleep(delays.getDelay(_IDX)); \
				break; \
			case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS: \
				o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
				ids.mutex.lock();\
				ids.addID(_IDX, std::this_thread::get_id());\
				ids.mutex.unlock(); \
			case ProfilerStatus::GCOZ_WAIT : \
				o##_NAME(PARAMETER_NAMES(__VA_ARGS__));	\
				break; \
		} \
	}
	D3D11_METHODS_VOID
#undef X

typedef HRESULT(__stdcall* Present)(IDXGISwapChain*, UINT, UINT);
static Present oPresent = NULL;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	// ImGui Setup
	//static bool init = false;
	//if (!init){
	//	GUI::init(pSwapChain);
	//	init = true;
	//}

	static unsigned long long int callCount = 0;
	//MethodDurations::Timepoint start;

	switch (man.getStatus()) {
	case ProfilerStatus::GCOZ_MEASURE: // measure times of D3D11 Methods, nothing else
		//MethodDurations::presentStart();// no need to track frames here
		if (callCount++ == MEASURE_FRAME_COUNT) {
			callCount = 0;
			delays.resetDelays();
			man.setStatus(ProfilerStatus::GCOZ_WAIT);
			com.announceFinish();
			man.announceStatusChange();
		}
		//MethodDurations::presentEnd();
		break;

	case ProfilerStatus::GCOZ_PROFILE: // apply last received delays and measure FPS
		durations.presentStart(man.getMethod(), man.getDelay());
		if (callCount++ == MEASURE_FRAME_COUNT + 2) { // +2 to make sure begin/end time of present is initialized
			callCount = 0;
			delays.resetDelays();
			man.setStatus(ProfilerStatus::GCOZ_WAIT);
			com.announceFinish(); 
			man.announceStatusChange();
		}
		durations.presentEnd();
		break;

	case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS:
		//MethodDurations::presentStart(man.getMethod(), man.getDelay()); // no need to track frames here
		ids.mutex.lock();
		ids.addID(8, std::this_thread::get_id());
		ids.mutex.unlock();
		if (callCount++ == METHOD_THREAD_COLLECTION_FRAME_COUNT) {
			callCount = 0;
			man.setStatus(ProfilerStatus::GCOZ_WAIT);
			com.announceFinish(); 
			man.announceStatusChange();
		}
		//MethodDurations::presentEnd();
		break;

	case ProfilerStatus::GCOZ_WAIT: // do nothing, wait for message from Profiler
		if (com.newDataAvailable()) {
			//DisplayInfoBox(L"Present Hook", L"newDataAvailable() == true");
			ProfilerMessage newData = com.getMessage();
			//DisplayInfoBox(L"Present Hook", L"Data received");
			//std::this_thread::sleep_for(std::chrono::seconds(5));
			//DisplayInfoBox(L"Present Hook", L"Data received");
			if (newData.valid) {
				//DisplayInfoBox(L"Present Hook", L"new Data is valid");
				man.waitNewStatus();
				if (man.getStatus() == ProfilerStatus::GCOZ_MEASURE) {
					try {
						delays.updateDelays(newData.delays);
						//DisplayInfoBox(L"Present Hook", L"updated delays");
					}
					catch (...) {
						DisplayErrorBox(L"Dll Main");
					}
				}
			}
		}
		break;
	default:
		//DisplayInfoBox(L"Dll Main", L"default case in Present switch");
		break;
	} // switch(ProfilerStatusManager::currentStatus)

	//GUI::showGCOZgui(man, overlayDebugMessage);

	return oPresent(pSwapChain, SyncInterval, Flags);
}

void hookD3D11() {
	#define X(idx, returnType, name, ...)\
			kiero::bind(idx, (void**)&o##name, hk##name);
			D3D11_METHODS
			D3D11_METHODS_VOID
	#undef X
	kiero::bind(8, (void**)&oPresent, hkPresent);
}
