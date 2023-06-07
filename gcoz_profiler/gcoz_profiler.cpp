// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "gcoz_profiler.h"

int main(int argc, char* argv[]) {

	wchar_t dllPath[] = L"C:\\Users\\Moritz\\Documents\\bachelor_thesis\\github\\gcoz\\gcoz\\x64\\Debug\\gcoz_dll.dll"; // prob needs abs path

	// using switch here is dumb
	if (argc < 2) {
		std::cout << err << "Provide PID";
	}

	DWORD PID = atoi(argv[1]);

	Injector injector = Injector(dllPath);
	injector.inject_dll(PID);

	std::cout << ok << "Done, exiting..." << std::endl;
	return 0;
}