// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <kiero.h>
#include "D3D11Hooks.h" //why does moving this down cause a compilation error???
#include <SharedMemoryBoost.hpp>
#include "ErrorMessage.h"

int profilerThread() {
	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		hookD3D11();
	}
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

