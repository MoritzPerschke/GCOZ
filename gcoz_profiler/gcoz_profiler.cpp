// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "gcoz_profiler.h"

int main(int argc, char* argv[]) {

	// using switch here is dumb
	if (argc < 2) {
		std::cout << err << "Provide PID";
	}

	DWORD PID = atoi(argv[1]);

	/* Communication */
	Communication com;
	try {
		com.init();
	}
	catch (const std::runtime_error& ex) {
		std::cout << err << ex.what() << std::endl;
		return 1;
	}

	/* Injection */
	// this HAS TO happen after Communication is initialized
	Injector injector = Injector();
	injector.inject_dll(PID);

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
			break;
		}
		
	}
	std::cout << ok << "Done, exiting..." << std::endl;
	return 0;
}