// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <d3d11.h>
#include "kiero/kiero.h"

#include "status.hpp"

typedef long(__stdcall* Present)(IDXGISwapChain*, UINT, UINT);
static Present oPresent = NULL;

long __stdcall hkPresent11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static bool init = false;

	if (!init)
	{
        MessageBoxW(0, L"Boom", L"works :D", MB_SETFOREGROUND);
		init = true;
	}


	return oPresent(pSwapChain, SyncInterval, Flags);
}

int kieroExampleThread() {
    if (kiero::init(kiero::RenderType::Auto) == kiero::Status::Success) {
        kiero::bind(8, (void**)&oPresent, hkPresent11);
        oPresent = (Present)kiero::getMethodsTable()[8];
    }
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    DisableThreadLibraryCalls(hModule);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)kieroExampleThread, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

