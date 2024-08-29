#include "Injector.h"

int Injector::inject_dll(DWORD _PID) {

	// handle to injected process and loadlibrary thread
	HANDLE hProcess, hThread = NULL;
	// handle to Kernel32 handle
	HMODULE hKernel32		 = NULL;
	// handle to buffer for dll
	PVOID rBuffer			 = NULL;
	// PID of injected process and loadlibrary thread
	DWORD PID, TID			 = NULL;

	PID = _PID;

	// get handle to process with provided PID
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (hProcess == NULL) {
		spdlog::error("Couldn't get handle to Process: {}", GetLastError());
		return 1;
	}

	// allocate memory for dll
	rBuffer = VirtualAllocEx(hProcess, rBuffer, sizeof(dllPath), (MEM_RESERVE | MEM_COMMIT), PAGE_READWRITE);
	if (rBuffer == NULL) {
		spdlog::error("Couldn't allocate remote Memory");
		return 1;

	}

	// write dll to remote path
	WriteProcessMemory(hProcess, rBuffer, (LPVOID)dllPath, sizeof(dllPath), NULL);

	// get Kernel32
	hKernel32 = GetModuleHandle(L"Kernel32");
	if (hKernel32 == NULL) {
		spdlog::error("Couldn't get Kernel32 handle: {}", GetLastError());
		return 1;
	}

	// LoadLibraryW as start routine for remote thread
	LPTHREAD_START_ROUTINE loadlib = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
	spdlog::info("Got address of loadLibrayW");

	// create and start the thread
	hThread = CreateRemoteThread(hProcess, NULL, 0, loadlib, rBuffer, 0, &TID);
	if (hThread == NULL) {
		spdlog::error("Couldn't get address of Thread: {}", GetLastError());
		return 1;
	}

	// wait for loading to finish
	spdlog::info("Remote thread creation success!");
	WaitForSingleObject(hThread, INFINITE);

	spdlog::info("Injection complete, cleaning up...");
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return 0;
}