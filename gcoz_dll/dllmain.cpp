// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <d3d11.h>
#include <Windows.h>
#include "kiero/kiero.h"
#include "../gcoz_profiler/Messages.h"

#define d3dPointer

/* typedef function pointer to Present function */
typedef HRESULT(__stdcall* Present)(IDXGISwapChain*, UINT, UINT);
/* save pointer to original Present function to call later */
static Present oPresent = NULL;

/* FileMapping to communicate between dll and profiler */
HANDLE hFileMapping      = NULL;
LPVOID pSharedMemory     = NULL;
Message* pData		     = NULL;

HANDLE hDataWrittenEvent = NULL;

HRESULT __stdcall hkPresent11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static bool init = false;
	static Message presentMessage = {};

	if (!init)
	{
		MessageBoxA(NULL, "skdfj", "sdkfj", MB_SETFOREGROUND);
		presentMessage.valid = true;
		presentMessage.function_from_table = 8;
		presentMessage.num_called		   = 0;
		init = true;
	}

	presentMessage.num_called++;
	*pData = presentMessage;
	SetEvent(hDataWrittenEvent);

	return oPresent(pSwapChain, SyncInterval, Flags);
}

int kieroExampleThread() {
	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		kiero::bind(8, (void**)&oPresent, hkPresent11); // don't use the 'getMethodstable' variant here
	}

	HANDLE hWriteFileMapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "gcoz_dll");
	pSharedMemory = MapViewOfFile(hWriteFileMapping, FILE_MAP_WRITE, 0, 0, 0);
	pData = static_cast<Message*>(pSharedMemory);
	hDataWrittenEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, "dllWrittenEvent");
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)kieroExampleThread, NULL, 0, NULL);
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:

	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

