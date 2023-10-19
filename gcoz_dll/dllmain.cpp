// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <d3d11.h> // C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um\d3d11.
#include <Windows.h>
#include <d3d11.h>
#include "../external/kiero/kiero.h"
#include "Communication.h"
#include "DelayManager.h"
#include "D3D11Hooks.h"
#include "../gcoz_profiler/Messages.h"

int profilerThread() {
	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		hookD3D11();
	}
	return 0;
}

int debugMessageBoxThread() {
	MessageBoxW(0, L"Debug", L"works", MB_SETFOREGROUND);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)profilerThread, NULL, 0, NULL); 
		//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)debugMessageBoxThread, NULL, 0, NULL);
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:

	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

