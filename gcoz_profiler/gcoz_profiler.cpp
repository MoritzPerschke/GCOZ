// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "gcoz_profiler.h"

using namespace std;

int main(int argc, char* argv[]) {

	wchar_t dllPath[] = L"C:\\Users\\Moritz\\Documents\\bachelor_thesis\\github\\gcoz\\gcoz\\x64\\Debug\\gcoz_dll.dll"; // prob needs abs path

	// using switch here is dumb
	if (argc < 2) {
		cout << e << "Provide PID";
	}

	DWORD PID = atoi(argv[1]);
	
	/* Communication */
	Communication com = Communication();
	cout << i << "GetLastError from Communication Setup: " << GetLastError() << endl;

	/* Injection */ //move to namespace/class
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (hProcess == NULL) {
		cout << e << "Couldn't get handle to Process: " << GetLastError() << endl;
		return 1;
	}

	PVOID rBuffer = NULL;
	rBuffer = VirtualAllocEx(hProcess, rBuffer, sizeof(dllPath), (MEM_RESERVE | MEM_COMMIT), PAGE_READWRITE);
	if (rBuffer == NULL) {
		cout << e << "Couldn't allocate remote Memory" << endl;
		return 1;

	}

	WriteProcessMemory(hProcess, rBuffer, (LPVOID)dllPath, sizeof(dllPath), NULL);
	cout << i << "Wrote to Process Memory" << endl;

	HMODULE hKernel32 = GetModuleHandle(L"Kernel32");
	if (hKernel32 == NULL) {
		cout << e << "Couldn't get Kernel32 handle: " << GetLastError() << endl;
		return 1;
	}

	LPTHREAD_START_ROUTINE loadlib = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
	cout << i << "Got address of loadLibrayW: " << loadlib << endl;

	DWORD TID;
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadlib, rBuffer, 0, &TID);
	if (hThread == NULL) {
		cout << e << "Couldn't get address of Thread: " << GetLastError();
		return 1;
	}

	cout << i << "Thread " << TID << "created with handle: " << hThread << ", waiting..." << endl;
	WaitForSingleObject(hThread, INFINITE);


	cout << i << "Thread finished, cleaning up..." << endl;
	CloseHandle(hThread);
	CloseHandle(hProcess);

	cout << i << "Done, exiting..." << endl;
	return 0;
}