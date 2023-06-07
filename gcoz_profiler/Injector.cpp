#include "Injector.h"

int Injector::inject_dll(DWORD _PID) {

	HANDLE hProcess, hThread = NULL;
	HMODULE hKernel32		 = NULL;
	PVOID rBuffer			 = NULL;
	DWORD PID, TID			 = NULL;

	PID = _PID;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (hProcess == NULL) {
		std::cout << err << "Couldn't get handle to Process: " << GetLastError() << std::endl;
		return 1;
	}

	rBuffer = VirtualAllocEx(hProcess, rBuffer, sizeof(dllPath), (MEM_RESERVE | MEM_COMMIT), PAGE_READWRITE);
	if (rBuffer == NULL) {
		std::cout << err << "Couldn't allocate remote Memory" << std::endl;
		return 1;

	}

	WriteProcessMemory(hProcess, rBuffer, (LPVOID)dllPath, sizeof(dllPath), NULL);
	std::cout << inf << "Wrote " << dllPath << " to Memory of Process " << PID << std::endl;

	hKernel32 = GetModuleHandle(L"Kernel32");
	if (hKernel32 == NULL) {
		std::cout << err << "Couldn't get Kernel32 handle: " << GetLastError() << std::endl;
		return 1;
	}

	LPTHREAD_START_ROUTINE loadlib = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
	std::cout << inf << "Got address of loadLibrayW: " << loadlib << std::endl;

	hThread = CreateRemoteThread(hProcess, NULL, 0, loadlib, rBuffer, 0, &TID);
	if (hThread == NULL) {
		std::cout << err << "Couldn't get address of Thread: " << GetLastError();
		return 1;
	}

	std::cout << inf << "Thread " << TID << " created with handle: " << hThread << " waiting..." << std::endl;
	WaitForSingleObject(hThread, INFINITE);

	std::cout << inf << "Thread finished, cleaning up..." << std::endl;
	CloseHandle(hThread);
	CloseHandle(hProcess);

	std::cout << ok << "Injection complete" << std::endl;

	return 0;
}