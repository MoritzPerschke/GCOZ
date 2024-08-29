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
std::string overlayDebugMessage = "Looking good :)";

// Busy waiting function to simulate the function taking longer
void little_sleep(Nanoseconds _delay) { // https://stackoverflow.com/a/45571538
	auto start = durations.now();
	auto end = start + _delay;
	while (durations.now() < end) {
		if ((durations.now() - end) > std::chrono::milliseconds(1)) {
			std::this_thread::yield();
		}
	}
}

/* The macros below use the definitions in 'HelperMacros.h' to generate whats needed for kiero hooking */
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
/*
 Depending on the current status, each hook function can
 1. measure execution time
 2. busy wait to do the profiling
 3. collect information about which thread calls which function
 4. do nothing
*/ 
#define X(_IDX, _RETURN_TYPE, _NAME, ...) \
	_RETURN_TYPE __stdcall hk##_NAME(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
		_RETURN_TYPE value; \
		Timepoint start; \
		Timepoint end; \
		switch (man.getStatus()) { \
			case ProfilerStatus::GCOZ_MEASURE : \
				start = durations.now(); \
				value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
				end = durations.now(); \
				RawDuration duration = end - start; \
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
		Timepoint end; \
		switch (man.getStatus()) { \
			case ProfilerStatus::GCOZ_MEASURE : \
				start = durations.now(); \
				o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
				end = durations.now(); \
				RawDuration duration = end - start; \
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

/* At the end of every rendered frame the 'Present' Method is called, which is why its hook is used to 
   communicate with the profiler and control the actions of the dll
*/
typedef HRESULT(__stdcall* Present)(IDXGISwapChain*, UINT, UINT);
static Present oPresent = NULL;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	static int callCount = 0;

	HRESULT value;
	//MethodDurations::Timepoint start;

	switch (man.getStatus()) {
	case ProfilerStatus::GCOZ_MEASURE: // measure times of D3D11 Methods, nothing else
		if (callCount++ == MEASURE_FRAME_COUNT) {
			callCount = 0;
			delays.resetDelays();
			man.setStatus(ProfilerStatus::GCOZ_WAIT);
			com.announceFinish();
			man.announceStatusChange();
		}
		value = oPresent(pSwapChain, SyncInterval, Flags);
		break;

	case ProfilerStatus::GCOZ_PROFILE: // apply last received delays and measure FPS
		durations.presentStart(man.getMethod(), man.getDelay(), callCount);
		if (callCount++ == MEASURE_FRAME_COUNT) {
			callCount = 0;
			durations.finish(man.getMethod(), man.getDelay());
			delays.resetDelays();
			man.setStatus(ProfilerStatus::GCOZ_WAIT);
			com.announceFinish(); 
			man.announceStatusChange();
			durations.reset();
		}
		value = oPresent(pSwapChain, SyncInterval, Flags);
		break;

	case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS:
		ids.mutex.lock();
		ids.addID(8, std::this_thread::get_id());
		ids.mutex.unlock();
		if (callCount++ >= static_cast<int>(METHOD_THREAD_COLLECTION_FRAME_COUNT / 100)) {
			callCount = 0;
			man.setStatus(ProfilerStatus::GCOZ_WAIT);
			com.announceFinish(); 
			man.announceStatusChange();
		}
		value = oPresent(pSwapChain, SyncInterval, Flags);
		break;

	case ProfilerStatus::GCOZ_WAIT: // do nothing, wait for message from Profiler
		if (com.newDataAvailable()) {
			ProfilerMessage newData = com.getMessage();
			if (newData.valid) {
				man.waitNewStatus();
				try {
					delays.updateDelays(newData.delays);
					//DisplayInfoBox(L"Present Hook", L"updated delays");
				}
				catch (...) {
					DisplayErrorBox(L"Dll Main");
				}
			}
		}
		value = oPresent(pSwapChain, SyncInterval, Flags);
		break;
	default:
		break;
	} // switch(ProfilerStatusManager::currentStatus)

	durations.presentEnd();
	return value;
}

void hookD3D11() {
	#define X(idx, returnType, name, ...)\
			kiero::bind(idx, (void**)&o##name, hk##name);
			D3D11_METHODS
			D3D11_METHODS_VOID
	#undef X
	kiero::bind(8, (void**)&oPresent, hkPresent);
}
