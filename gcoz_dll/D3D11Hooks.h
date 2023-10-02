#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <thread>

#include "ErrorMessage.h"
#include "HelperMacros.h"
#include "DelayManager.h"
#include "MethodDurations.h"
#include "ProfilerStatusManager.h"
#include "../gcoz_profiler/Constants.h"

namespace D3D11Hooks {
	DelayManager delays; // not sure if this is the best option, default constructor sets all delays to 0
	Communication com = Communication();

	void little_sleep(Nanoseconds _delay) { // https://stackoverflow.com/a/45571538
		auto start = MethodDurations::now();
		auto end = start + _delay;
		while (MethodDurations::now() < end) {
			if ((MethodDurations::now() - end) > std::chrono::milliseconds(1)){
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
			MethodDurations::Timepoint start; \
			switch (ProfilerStatusManager::currentStatus) { \
				case ProfilerStatus::GCOZ_MEASURE : \
					start = MethodDurations::now(); \
					value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
					MethodDurations::Duration duration = MethodDurations::now() - start; \
					MethodDurations::addDuration(_IDX, duration); \
					break; \
				case ProfilerStatus::GCOZ_PROFILE : \
					value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
					little_sleep(delays.getDelay(_IDX)); \
					break; \
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
			MethodDurations::Timepoint start; \
			switch (ProfilerStatusManager::currentStatus) { \
				case ProfilerStatus::GCOZ_MEASURE : \
					start = MethodDurations::now(); \
					o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
					MethodDurations::Duration duration = MethodDurations::now() - start; \
					MethodDurations::addDuration(_IDX, duration); \
					break; \
				case ProfilerStatus::GCOZ_PROFILE : \
					o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
					little_sleep(delays.getDelay(_IDX)); \
					break; \
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
		static unsigned long long int callCount = 0;
		HRESULT value;
		MethodDurations::Timepoint start;

		switch (ProfilerStatusManager::currentStatus) {
			case ProfilerStatus::GCOZ_MEASURE : // measure times of D3D11 Methods, nothing else
				MethodDurations::presentStart();
				if (callCount++ == MEASURE_FRAME_COUNT) { // this could prob be done in seperate thread
					callCount = 0;
					DllMessage send = {};
					// remove measurement of frametimes, use allMethods[0%] as baseline
					//send.frameTimes = MethodDurations::getPresentTimes();
					//send.frameRates = MethodDurations::getFrameRates();
					send.durations = MethodDurations::getDurations();
					send.methodCalls = MethodDurations::getCallAmounts();
					send.lastStatus = ProfilerStatusManager::currentStatus;
					send.valid = true;
					com.sendMessage(send);
					ProfilerStatusManager::changeStatus(ProfilerStatus::GCOZ_WAIT);
				}
				value = oPresent(pSwapChain, SyncInterval, Flags);
				MethodDurations::presentEnd();
				break;

			case ProfilerStatus::GCOZ_PROFILE : // apply last received delays and measure FPS
				MethodDurations::presentStart();
				if (callCount++ == MEASURE_FRAME_COUNT) {
					callCount = 0;
					DllMessage send = {};
					send.lastStatus = ProfilerStatusManager::currentStatus;
					send.frameTimes = MethodDurations::getPresentTimes();
					send.frameRates = MethodDurations::getFrameRates();
					send.valid = true;
					com.sendMessage(send);
					ProfilerStatusManager::changeStatus(ProfilerStatus::GCOZ_WAIT);
				}
				value = oPresent(pSwapChain, SyncInterval, Flags);
				MethodDurations::presentEnd();
				break;

			case ProfilerStatus::GCOZ_WAIT : // do nothing, wait for message from Profiler
				delays.resetDelays();
				ProfilerMessage profilerResponse = com.getMessage(1);
				if (profilerResponse.valid) {
					if (profilerResponse.status == ProfilerStatus::GCOZ_PROFILE) {
						delays.updateDelays(profilerResponse.delays); // in case status is any measure, set these to 0
					}
					if (profilerResponse.status != ProfilerStatusManager::currentStatus) {
						ProfilerStatusManager::changeStatus(profilerResponse.status);
					}
				}
				else {
					//DisplayErrorBox(L"Updating Delays", L"Failed to get updated delays from profiler");
				}
				value = oPresent(pSwapChain, SyncInterval, Flags);
				break;
		} // switch(ProfilerStatusManager::currentStatus)
		return value;
	}

	void hookD3D11() {
		#define X(idx, returnType, name, ...)\
				kiero::bind(idx, (void**)&o##name, hk##name);
			D3D11_METHODS
			D3D11_METHODS_VOID
		#undef X
		kiero::bind(8, (void**)&oPresent, hkPresent);
		DisplayInfoBox(L"Progress", L"D3D11 functions hooked");
	}

} // namespace D3D11Hooks


	