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
	#define X(idx, returnType, name, ...) \
		returnType __stdcall hk##name(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
			std::this_thread::sleep_for(std::chrono::milliseconds(delays.getDelay(idx)));\
			MethodDurations::Timepoint start = MethodDurations::now();\
			returnType value = o##name(PARAMETER_NAMES(__VA_ARGS__)); \
			MethodDurations::Duration duration = MethodDurations::now() - start;\
			MethodDurations::addDuration(idx, duration);\
			return value; \
		}
		D3D11_METHODS
	#undef X

	/* same as above just for void functions */
	#define X(idx, returnType, name, ...) \
		returnType __stdcall hk##name(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
			std::this_thread::sleep_for(std::chrono::milliseconds(delays.getDelay(idx)));\
			MethodDurations::Timepoint start = MethodDurations::now();\
			o##name(PARAMETER_NAMES(__VA_ARGS__)); \
			MethodDurations::Duration duration = MethodDurations::now() - start;\
			MethodDurations::addDuration(idx, duration);\
		}
		D3D11_METHODS_VOID
	#undef X

	typedef HRESULT(__stdcall* Present)(IDXGISwapChain*, UINT, UINT);
	static Present oPresent = NULL;
	HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
		MethodDurations::presentCalled();
		
		static unsigned long long int callCount = 0;
		callCount++;

		std::this_thread::sleep_for(std::chrono::milliseconds(delays.getDelay(8))); // maybe make this more readable ('using')

		MethodDurations::Timepoint start = MethodDurations::now();
		HRESULT value = oPresent(pSwapChain, SyncInterval, Flags);
		MethodDurations::Duration duration = MethodDurations::now() - start;
		MethodDurations::addDuration(8, duration);

		if (callCount % 500 == 0) {
			// update the profiler with the average execution times
			DllMessage send = {};
			if (MethodDurations::getPresentTimes(send) == 0) { // this does not take, profiler still gets 0-length vector
				DisplayErrorBox(L"Sending Message", L"frameTimePoints vector is empty");
			}
			send.durations = MethodDurations::getDurations();
			send.valid = true;
			if (!com.sendMessage(send)) {
				DisplayErrorBox(L"Sending Message to Profiler");
			}

			// update the delays (maybe this should happen in Delaymanager Class)
			ProfilerMessage newDelays = com.getMessage(0);
			if (newDelays.valid) {
				delays.updateDelays(newDelays.delays);
			}
			else {
				//DisplayErrorBox(L"Updating Delays", L"Failed to get updated delays from profiler");
			}
		}

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


	