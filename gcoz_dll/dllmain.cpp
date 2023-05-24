// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <d3d9.h>
#include "kiero/kiero.h"

typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
static EndScene oEndScene = NULL;

long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice) {
    static bool init = false;

    if (!init) {
        MessageBox(0, L"Boom it works", L"Kiero", MB_OK);
        init = true;
    }

    return oEndScene(pDevice);
}

int kieroExampleThread() {
    if (kiero::init(kiero::RenderType::D3D9)) {
        kiero::bind(42, (void**)&oEndScene, hkEndScene);
        oEndScene = (EndScene)kiero::getMethodsTable()[42];
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

