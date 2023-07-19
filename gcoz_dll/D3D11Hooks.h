#pragma once
#include "HelperMacros.h"
#include <dxgi.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>

DWORD methodDelays[205];
namespace D3D11Hooks{

	// expands to function pointers for D3D11 methods
	#define X(idx, returnType, name, ...) typedef returnType (__stdcall* name)(PARAMETER_TYPES(__VA_ARGS__));
		D3D11_METHODS
		D3D11_METHODS_VOID
	#undef X

	// expands to settin previously defined pointers to NULL
	#define X(idx, returnType, name, ...) static name o##name = NULL;
			D3D11_METHODS
			D3D11_METHODS_VOID
	#undef X

	// expands to hooked d3d11 function
	#define X(idx, returnType, name, ...) \
		returnType __stdcall hk##name(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
			Sleep(methodDelays[idx]); \
			returnType value = o##name(PARAMETER_NAMES(__VA_ARGS__)); \
			return value; \
		}
		D3D11_METHODS
	#undef X

	// same as above just for void functions
	#define X(idx, returnType, name, ...) \
		returnType __stdcall hk##name(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
			Sleep(methodDelays[idx]); \
			o##name(PARAMETER_NAMES(__VA_ARGS__)); \
			return; \
		}
		D3D11_METHODS_VOID
	#undef X

	void hookD3D11() {
		#define X(idx, returnType, name, ...)\
			kiero::bind(idx, (void**)&o##name, hk##name);
			D3D11_METHODS
			D3D11_METHODS_VOID
		#undef X
	}
} // namespace D3D11Hooks


