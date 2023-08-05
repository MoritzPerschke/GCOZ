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
	while (true) {
		DllMessage msg = com.getMessage();
		if (msg.valid) {
			receivedMsgs++;
			std::cout << ok << "Got times from dll:" << std::endl;
			for (int i = 0; i < 205; i++) {
				if (msg.durations[i] > std::chrono::nanoseconds(0)) {
					std::cout << inf << i << ": " << msg.durations[i].count() << std::endl;
				}
			}
			std::cout << std::endl;
			std::cout << inf << "Vector of Present Durations is: " << msg.frameTimepoints.size() << " elements long" << std::endl;
			for (const auto& iter : msg.frameTimepoints) {
				std::cout << iter.count() << std::endl; // this does not get printed?
			}
			
			//if (receivedMsgs % 10 == 0) {
			//	ProfilerMessage send = {};
			//	send.status = ProfilerStatus::GCOZ_PROFILE;
			//	send.delays.fill(1);
			//	send.valid = true;
			//	com.sendMessage(send);
			//}
			//else {
			//	ProfilerMessage send = {};
			//	send.status = ProfilerStatus::GCOZ_MEASURE;
			//	send.delays.fill(0);
			//	send.valid = true;
			//	com.sendMessage(send);
			//}
		} // if(msg.valid)
		else {
			std::cout << err << "No message after timeout" << std::endl;
			//break;
		}
		
	}
	std::cout << ok << "Done, exiting..." << std::endl;
	return 0;
}