// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "gcoz_profiler.h"

int main(int argc, char* argv[]) {

	// using switch here is dumb
	if (argc < 3) {
		std::cout << err << "Usage: .\\gcoz_profiler.exe </data subdirectory> <PID>";
	}
	DWORD PID = atoi(argv[2]);
	string processName = string(argv[1]);

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

	/* Main profiling loop */
	std::cout << inf << "Waiting for game to be in steady state" << std::endl;
	system("pause");
	std::cout << inf << "Waiting 10 more seconds" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(10)); // wait to get into steady state and wait 10 to tab back into game

	ProfilerStatusManager man = ProfilerStatusManager(processName);
	std::cout << ok << "Starting profiling" << std::endl;
	do {
		DllMessage msg = com.getMessage();
		if (msg.valid) {
			ProfilerMessage nextMsg;
			man.next(msg, nextMsg);
			com.sendMessage(nextMsg);
		} // if(msg.valid)
		else {
			std::cout << err << "No message after timeout" << std::endl;
			break;
		}
	} while (!man.dataCollected());
	man.finish();

	std::cout << ok << "Done, exiting..." << std::endl;
	return 0;
}