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

	/* Communication */
	Communication com = Communication();
	std::cout << inf << "GetLastError from Communication Setup: " << GetLastError() << std::endl;


	while (true) {
		DllMessage msg = com.getMessage();
		if (msg.valid) {
			std::cout << inf << "Got times from dll:" << std::endl;
			for (int i = 0; i < 205; i++) {
				std::cout << i << ": " << msg.durations[i].count() << "\n";
			}
		}
		else {
			std::cout << err << "No message after timeout" << std::endl;
		}
		
	}
	std::cout << ok << "Done, exiting..." << std::endl;
	return 0;
}