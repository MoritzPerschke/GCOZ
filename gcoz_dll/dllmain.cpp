// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <d3d11.h> // C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um\d3d11.h
#include <Windows.h>
#include <chrono>
#include "kiero/kiero.h"
#include "../gcoz_profiler/Messages.h"
#include "D3D11Hooks.h"

/* FileMapping to communicate between dll and profiler */
Message* sharedMemoryRead   = NULL;
Response* sharedMemoryWrite = NULL;
HANDLE hDataWrittenEvent    = NULL;


int profilerThread() {
	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		kiero::bind(8, (void**)&oPresent, hkPresent); // don't use the 'getMethodstable' variant here
	}

	HANDLE hReadFileMapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "gcoz_dll"); // read from here
	LPVOID pReadSharedMemory = MapViewOfFile(hReadFileMapping, FILE_MAP_READ, 0, 0, 0);
	sharedMemoryRead = static_cast<Message*>(pReadSharedMemory);

	HANDLE hWriteFileMapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "gcoz_profiler");
	LPVOID pWriteSharedMemory = MapViewOfFile(hWriteFileMapping, FILE_MAP_WRITE, 0, 0, 0);
	sharedMemoryWrite = static_cast<Response*>(pWriteSharedMemory);

	hDataWrittenEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, "dllWrittenEvent");
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)profilerThread, NULL, 0, NULL);
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:

	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

