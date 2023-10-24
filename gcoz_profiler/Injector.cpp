#include "Injector.h"

int Injector::inject_dll(DWORD _PID) {

	HANDLE hProcess, hThread = NULL;
	HMODULE hKernel32		 = NULL;
	PVOID rBuffer			 = NULL;
	DWORD PID, TID			 = NULL;

	PID = _PID;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (hProcess == NULL) {
		spdlog::error("Couldn't get handle to Process: {}", GetLastError());
		return 1;
	}

	rBuffer = VirtualAllocEx(hProcess, rBuffer, sizeof(dllPath), (MEM_RESERVE | MEM_COMMIT), PAGE_READWRITE);
	if (rBuffer == NULL) {
		spdlog::error("Couldn't allocate remote Memory");
		return 1;

	}

	WriteProcessMemory(hProcess, rBuffer, (LPVOID)dllPath, sizeof(dllPath), NULL);

	hKernel32 = GetModuleHandle(L"Kernel32");
	if (hKernel32 == NULL) {
		spdlog::error("Couldn't get Kernel32 handle: {}", GetLastError());
		return 1;
	}

	LPTHREAD_START_ROUTINE loadlib = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
	spdlog::info("Got address of loadLibrayW");

	hThread = CreateRemoteThread(hProcess, NULL, 0, loadlib, rBuffer, 0, &TID);
	if (hThread == NULL) {
		spdlog::error("Couldn't get address of Thread: {}", GetLastError());
		return 1;
	}

	spdlog::info("Remote thread creation success!");
	WaitForSingleObject(hThread, INFINITE);

	spdlog::info("Injection complete, cleaning up...");
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return 0;
}