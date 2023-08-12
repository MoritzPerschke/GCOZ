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

namespace D3D11Hooks{
	DelayManager delays; // not sure if this is the best option, default constructor sets all delays to 0
	Communication com = Communication();

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
					std::this_thread::sleep_for(std::chrono::nanoseconds(delays.getDelay(_IDX))); \
					value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
					break; \
				case ProfilerStatus::GCOZ_WAIT : \
					value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__));	\
					break; \
				default : \
					value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
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
					std::this_thread::sleep_for(std::chrono::nanoseconds(delays.getDelay(_IDX))); \
					o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
					break; \
				case ProfilerStatus::GCOZ_WAIT : \
					o##_NAME(PARAMETER_NAMES(__VA_ARGS__));	\
					break; \
				default : \
					o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
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
				if (++callCount == MEASURE_FRAME_COUNT) {
					callCount = 0;
					DllMessage send = {};
					MethodDurations::getPresentTimes(send);
					MethodDurations::getDurations(send);
					send.lastStatus = ProfilerStatusManager::currentStatus;
					send.valid = true;
					com.sendMessage(send);
					//ProfilerStatusManager::changeStatus(ProfilerStatus::GCOZ_WAIT);
				}
				MethodDurations::presentCalled();
				start = MethodDurations::now();
				value = oPresent(pSwapChain, SyncInterval, Flags);
				MethodDurations::Duration duration = MethodDurations::now() - start;
				MethodDurations::addDuration(8, duration);
				
				break;

			case ProfilerStatus::GCOZ_PROFILE : // apply last received delays and measure FPS
				if (++callCount == MEASURE_FRAME_COUNT) {
					callCount = 0;
					DllMessage send = {};
					MethodDurations::getPresentTimes(send);
					MethodDurations::getDurations(send);
					send.lastStatus = ProfilerStatusManager::currentStatus;
					send.valid = true;
					com.sendMessage(send);
					//ProfilerStatusManager::changeStatus(ProfilerStatus::GCOZ_WAIT);
				}
				MethodDurations::presentCalled();
				std::this_thread::sleep_for(std::chrono::nanoseconds(delays.getDelay(8))); // prob. use Ns here, Ms drops FPS to <1
				value = oPresent(pSwapChain, SyncInterval, Flags);
				break;

			case ProfilerStatus::GCOZ_WAIT : // do nothing, wait for message from Profiler
				delays.resetDelays();
				ProfilerMessage profilerResponse = com.getMessage(1);
				if (profilerResponse.valid) {
					delays.updateDelays(profilerResponse.delays);
					if (profilerResponse.status != ProfilerStatusManager::currentStatus) {
						ProfilerStatusManager::changeStatus(profilerResponse.status);
					}
				}
				else {
					//DisplayErrorBox(L"Updating Delays", L"Failed to get updated delays from profiler");
				}
				value = oPresent(pSwapChain, SyncInterval, Flags);
				break;

			default:
				value = oPresent(pSwapChain, SyncInterval, Flags);
		} // switch(ProfilerStatusManager::currentStatus)
		return value;
	}

	void hookD3D11() {
		kiero::bind(8, (void**)&oPresent, hkPresent);
		#define X(idx, returnType, name, ...)\
				kiero::bind(idx, (void**)&o##name, hk##name);
			D3D11_METHODS
			D3D11_METHODS_VOID
		#undef X
		DisplayInfoBox(L"Progress", L"D3D11 functions hooked");
	}
} // namespace D3D11Hooks


	