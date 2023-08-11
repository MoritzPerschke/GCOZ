// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

	int receivedMsgs = 0;
	while (true) { /// TODO: change this
		DllMessage msg = com.getMessage();
		if (msg.valid) {
			switch (msg.lastStatus) {
			case ProfilerStatus::GCOZ_MEASURE:
				std::cout << ok << "Measured method durations:" << std::endl;
				for (int i = 0; i < msg.durations.size(); i++) {
					if (msg.durations[i] != std::chrono::nanoseconds(0)) {
						std::cout << inf << i << ": " << msg.durations[i].count() << std::endl;
					}
				}
				std::cout << std::endl;
				break;
			case ProfilerStatus::GCOZ_PROFILE:
				std::cout << ok << "FrameDurations: " << std::endl;
				for (int i = 0; i < MEASURE_FRAME_COUNT; i++) {
					std::cout << inf << i << ": " << msg.frameTimepoints[i].count() << std::endl;
				}
				break;
			case ProfilerStatus::GCOZ_WAIT: // this should never happen, GCOZ_WAIT doesn't send any messages to profiler
				break;
			} // switch(msg.lastStatus
		} // if(msg.valid)
		else {
			std::cout << err << "No message after timeout" << std::endl;
			break;
		}
		
	}
	std::cout << ok << "Done, exiting..." << std::endl;
	return 0;
}