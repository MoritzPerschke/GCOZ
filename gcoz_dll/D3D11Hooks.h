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
	Communication com = Communication();

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
			returnType value = o##name(PARAMETER_NAMES(__VA_ARGS__)); \
			return value; \
		}
		D3D11_METHODS
	#undef X // this hook does not work, crashes game

	// same as above just for void functions
	#define X(idx, returnType, name, ...) \
		returnType __stdcall hk##name(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
			o##name(PARAMETER_NAMES(__VA_ARGS__)); \
			return; \
		}
		D3D11_METHODS_VOID
	#undef X

	typedef HRESULT(__stdcall* SetPrivateData)(REFGUID, UINT, const void*);
	static SetPrivateData oSetPrivateData = NULL;
	HRESULT __stdcall hkSetPrivateData(REFGUID guid, UINT DataSize, const void* pData) {
		/*Sleep(delays.getDelay(3));*/
		MessageBoxW(0, L"Debug", L"SetPrivateData1", MB_SETFOREGROUND);

		MethodDurations::Timepoint start = MethodDurations::now();
		HRESULT value = oSetPrivateData(guid, DataSize, pData);
		MessageBoxW(0, L"Debug", L"SetPrivateData2", MB_SETFOREGROUND);

		MethodDurations::Duration duration = MethodDurations::now() - start;
		MethodDurations::addDuration(3, duration);
		return value;
	} //this hook works?

	typedef HRESULT(__stdcall* Present)(IDXGISwapChain*, UINT, UINT);
	static Present oPresent = NULL;
	HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
			static int callCount = 0;
			callCount++;
			
			MethodDurations::Timepoint start = MethodDurations::now();
			HRESULT value = oPresent(pSwapChain, SyncInterval, Flags);
			MethodDurations::Duration duration = MethodDurations::now() - start;
			MethodDurations::addDuration(8, duration);

			DllMessage send = {};
			send.durations = MethodDurations::getDurations();
			send.valid = true;
			com.sendMessage(send); // crashes game
			//MessageBoxW(0, L"hkPresent done", L"Debug", MB_SETFOREGROUND);

			return value;
	}

	//X(74, void, Draw, UINT, VertexCount, UINT, StartVertexLocation)
	typedef void(__stdcall* Draw)(UINT, UINT);
	static Draw oDraw = NULL;
	void __stdcall hkDraw(UINT VertexCount, UINT StartVertexLocation) {
		MessageBoxW(0, L"Debug", L"Draw1", MB_SETFOREGROUND);

		MethodDurations::Timepoint start = MethodDurations::now();
		oDraw(VertexCount, StartVertexLocation);
		MessageBoxW(0, L"Debug", L"Draw2", MB_SETFOREGROUND);
		MethodDurations::Duration duration = MethodDurations::now() - start;

		MethodDurations::addDuration(74, duration);
		return;
	}

	void hookD3D11() {
		kiero::bind(8, (void**)&oPresent, hkPresent);
		kiero::bind(3, (void**)&oSetPrivateData, hkSetPrivateData);
		//kiero::bind(74, (void**)&oDraw, hkDraw);

		//#define X(idx, returnType, name, ...)\
			//kiero::bind(idx, (void**)&o##name, hk##name);
			//D3D11_METHODS
			//D3D11_METHODS_VOID
		//#undef X
		MessageBoxW(0, L"hookD3D11 complete", L"Debug", MB_SETFOREGROUND);
	}
} // namespace D3D11Hooks


	