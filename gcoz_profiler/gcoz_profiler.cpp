// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "gcoz_profiler.h"

int main(int argc, char* argv[]) {

	// using switch here is dumb
	if (argc < 2) {
		std::cout << err << "Provide PID";
	}

	DWORD PID = atoi(argv[1]);

	Injector injector = Injector();
	injector.inject_dll(PID);

	std::cout << ok << "Done, exiting..." << std::endl;
	return 0;
}