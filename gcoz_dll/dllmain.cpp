// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <d3d11.h> // C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um\d3d11.h
#include <Windows.h>
#include <d3d11.h>
#include "kiero/kiero.h"
#include "Communication.h"
#include "DelayManager.h"
#include "D3D11Hooks.h"
#include "../gcoz_profiler/Messages.h"

int profilerThread() {
	Communication com = Communication();
	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		D3D11Hooks::hookD3D11(com);
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
	return TRUE;
}