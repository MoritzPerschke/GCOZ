// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "gcoz_profiler.h"

int main(int argc, char* argv[]) {
	HANDLE hProcess, hThread = NULL;
	HMODULE hKernel32		 = NULL;
	PVOID rBuffer			 = NULL;
	DWORD PID, TID			 = NULL;

	wchar_t dllPath[] = L"C:\\Users\\Moritz\\Documents\\bachelor_thesis\\github\\gcoz\\gcoz\\x64\\Debug\\gcoz_dll.dll"; // prob needs abs path

	// using switch here is dumb
	if (argc < 2) {
		std::cout << e << "Provide PID";
	}

	PID = atoi(argv[1]);

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (hProcess == NULL) {
		std::cout << e << "Couldn't get handle to Process: " << GetLastError() << std::endl;
		return 1;
	}

	rBuffer = VirtualAllocEx(hProcess, rBuffer, sizeof(dllPath), (MEM_RESERVE | MEM_COMMIT), PAGE_READWRITE);
	if (rBuffer == NULL) {
		std::cout << e << "Couldn't allocate remote Memory" << std::endl;
		return 1;

	}

	WriteProcessMemory(hProcess, rBuffer, (LPVOID)dllPath, sizeof(dllPath), NULL);
	std::cout << i << "Wrote to Process Memory" << std::endl;

	hKernel32 = GetModuleHandle(L"Kernel32");
	if (hKernel32 == NULL) {
		std::cout << e << "Couldn't get Kernel32 handle: " << GetLastError() << std::endl;
		return 1;
	}

	LPTHREAD_START_ROUTINE loadlib = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
	std::cout << i << "Got address of loadLibrayW: " << loadlib << std::endl;

	hThread = CreateRemoteThread(hProcess, NULL, 0, loadlib, rBuffer, 0, &TID);
	if (hThread == NULL) {
		std::cout << e << "Couldn't get address of Thread: " << GetLastError();
		return 1;
	}

	std::cout << i << "Thread " << TID << "created with handle: " << hThread << "waiting..." << std::endl;
	WaitForSingleObject(hThread, INFINITE);

	/* Communication */
	hFileMapping = OpenFileMapping(
		FILE_MAP_READ,
		FALSE,
		L"GCOZ_Shared_Memory"
	);
	if (hFileMapping == NULL) {
		std::cout << e << "Couldn't File Mapping..." << std::endl;
	}
	else {
		pSharedMemory = MapViewOfFile(
			hFileMapping,
			FILE_MAP_READ,
			0, 0, 0);
		if (pSharedMemory == NULL) {
			std::cout << e << "Couldn't Open Shared Memory" << std::endl;
		}
		else {
			pData = static_cast<Message*>(pSharedMemory);
			hDataWrittenEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"DataWrittenEvent");

			bool reading = true;
			while (reading) {
				DWORD dWaitResult = WaitForSingleObject(hDataWrittenEvent, INFINITE);

				if (dWaitResult == WAIT_OBJECT_0) {
					dllMessage = *pData;
					if (dllMessage.num_called % 10 == 0) {
						std::cout << i << "Function " << dllMessage.function_from_table << " has been called " << dllMessage.num_called << " Times" << std::endl;
						ResetEvent(hDataWrittenEvent);
					}
				}
			}

		}

	}
		 
	std::cout << i << "Thread finished, cleaning up..." << std::endl;
	UnmapViewOfFile(pSharedMemory);
	CloseHandle(hFileMapping);
	CloseHandle(hDataWrittenEvent);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	std::cout << i << "Done, exiting..." << std::endl;
	return 0;
}