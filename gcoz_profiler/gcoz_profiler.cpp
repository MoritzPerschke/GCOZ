// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <fileapi.h>


int main(int argc, char* argv[]) {
	HANDLE processHandle = NULL;
	PVOID remoteBuffer = NULL;
	DWORD processId = 7504;

	wchar_t dllPath[] = L"C:\\Users\\Moritz\\Documents\\bachelor_thesis\\github\\gcoz\\gcoz\\x64\\Debug\\gcoz_dll.dll"; // prob needs abs path

	// using switch here for later functionality
	switch (argc)
	{
	case 2:
		processId = DWORD(atoi(argv[1]));
		break;
	default:
		break;
		std::cout << "Usage: ./gcoz_profiler <ProcessId>" << std::endl;
		std::cout << "Provide a PID" << std::endl;
		std::cin >> processId;
	}



	processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	remoteBuffer = VirtualAllocEx(processHandle, NULL, sizeof(dllPath), MEM_COMMIT, PAGE_READWRITE);
	if (processHandle == NULL || remoteBuffer == NULL || processId == NULL) {
		std::cout << "Initialization failed" << std::endl;
		std::cout << "handle: " << processHandle << "buffer: " << remoteBuffer << "procID: " << processId << std::endl;
		std::cout << GetLastError() << std::endl;
		
	}

	if (WriteProcessMemory(processHandle, remoteBuffer, (LPVOID)dllPath, sizeof(dllPath), NULL)) {
		std::cout << "WriteProcessMemory success!" << std::endl;
	}

	PTHREAD_START_ROUTINE threatStartRoutineAddress = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"Kernel32"), "LoadLibraryW");
	std::cout << "Thread Start Routine Address: " << threatStartRoutineAddress << std::endl;

	HANDLE remoteThreadHandle = CreateRemoteThread(processHandle, NULL, 0, threatStartRoutineAddress, remoteBuffer, 0, NULL);
	std::cout << "Remote Thread Handle" << remoteThreadHandle << std::endl;

	/*LPDWORD rmExitCode = (LPDWORD)259;
	while (rmExitCode == (LPDWORD)259) {
		GetExitCodeProcess(processHandle, rmExitCode);
	}*/


	CloseHandle(processHandle);

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
