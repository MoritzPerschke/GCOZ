#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include "HelperMacros.h"
#include "DelayManager.h"
#include "MethodDurations.h"

namespace D3D11Hooks{
	DelayManager delays; // not sure if this is the best option, default constructor sets all delays to 0
	Communication com; // this might not be safe, default constructor is the only one

	// expands to function pointers for D3D11 methods
	#define X(idx, returnType, name, ...) typedef returnType (__stdcall* name)(PARAMETER_TYPES(__VA_ARGS__));
	D3D11_METHODS
		D3D11_METHODS_VOID
	#undef X

		// expands to setting previously defined pointers to NULL
	#define X(idx, returnType, name, ...) static name o##name = NULL;
		D3D11_METHODS
		D3D11_METHODS_VOID
	#undef X

		// expands to hooked d3d11 function
	#define X(idx, returnType, name, ...) \
		returnType __stdcall hk##name(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
			Sleep(delays.getDelay(idx)); \
			MethodDurations::Timepoint start = MethodDurations::now(); \
			returnType value = o##name(PARAMETER_NAMES(__VA_ARGS__)); \
			MethodDurations::Duration duration = MethodDurations::now() - start; \
			MethodDurations::addDuration(idx, duration); \
			return value; \
		}
		D3D11_METHODS
	#undef X

		// same as above just for void functions
	#define X(idx, returnType, name, ...) \
		returnType __stdcall hk##name(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
			Sleep(delays.getDelay(idx)); \
			o##name(PARAMETER_NAMES(__VA_ARGS__)); \
			return; \
		}
		D3D11_METHODS_VOID
	#undef X

	typedef HRESULT(__stdcall* Present)(IDXGISwapChain*, UINT, UINT);
	static Present oPresent = NULL;
	HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
			static int callCount = 0;
			callCount++;
			
			MethodDurations::Timepoint start = MethodDurations::now();
			HRESULT value = oPresent(pSwapChain, SyncInterval, Flags);
			MethodDurations::Duration duration = MethodDurations::now() - start;
			MethodDurations::addDuration(8, duration);

			DllMessage send;
			send.durations = MethodDurations::getDurations();
			send.valid = true;
			com.sendMessage(send);

			return value;
	}

	void hookD3D11(Communication _com) {
		com = _com;
		delays = DelayManager(_com);
		kiero::bind(8, (void**)&oPresent, hkPresent);
		#define X(idx, returnType, name, ...)\
			kiero::bind(idx, (void**)&o##name, hk##name);
			D3D11_METHODS
			D3D11_METHODS_VOID
		#undef X
	}
} // namespace D3D11Hooks


