// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <d3d11.h>
#include <Windows.h>
#include "kiero/kiero.h"
#include "Messages.h"

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
	static Message presentMessage;

	//MessageBoxW(0, L"Boom", L"works :D", MB_SETFOREGROUND);
	if (!init)
	{
		presentMessage.function_from_table = 8;
		presentMessage.num_called		   = 1;
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
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)kieroExampleThread, NULL, 0, NULL);

		hFileMapping = CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			1024,
			L"GCOZ_Shared_Memory"
		);
		if (hFileMapping == NULL) return 1;

		pSharedMemory = MapViewOfFile(
			hFileMapping,
			FILE_MAP_ALL_ACCESS,
			0, 0, 0);
		if (pSharedMemory == NULL) return 1;

		pData = static_cast<Message*>(pSharedMemory);

		hDataWrittenEvent = CreateEvent(
			NULL,
			FALSE,
			FALSE,
			L"DataWrittenEvent"
		);

		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:

	case DLL_PROCESS_DETACH:
		UnmapViewOfFile(pSharedMemory);
		CloseHandle(hFileMapping);
		CloseHandle(hDataWrittenEvent);
		break;
	}

	return TRUE;
}

