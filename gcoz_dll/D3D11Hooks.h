#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <thread>

#include "kiero/kiero.h"
#include "ErrorMessage.h"
#include "HelperMacros.h"
#include "DelayManager.h"
#include "MethodDurations.h"

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

DelayManager delays; // not sure if this is the best option, default constructor sets all delays to 0
Communication com = Communication();
static Present oPresent = NULL;

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
void hookD3D11();
