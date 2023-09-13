// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "gcoz_profiler.h"

int main(int argc, char* argv[]) {

	// using switch here is dumb
	if (argc < 3) {
		std::cout << inf << "Size of DllMessage: " << sizeof(DllMessage) << std::endl;
		std::cout << err << "Usage: .\\gcoz_profiler.exe </data subdirectory> <PID>";
	}
	DWORD PID = atoi(argv[2]);
	string processName = string(argv[1]);

	//try {
		/* Communication */
		Communication com;
		com.init();

		/* Injection */
		// this HAS TO happen after Communication is initialized
		Injector injector = Injector();
		injector.inject_dll(PID);

		/* Main profiling loop */
		std::cout << inf << "Waiting for game to be in steady state" << std::endl;
		system("pause");
			std::cout << inf << "Waiting 10 more seconds" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(10)); // wait to get into steady state and wait 10 to tab back into game

			std::cout << ok << "Starting profiling" << std::endl;
			ProfilerStatusManager man = ProfilerStatusManager(processName);
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

	//}
	//catch (const std::runtime_error& error) {
	//	std::cout << err << error.what() << std::endl;
	//	return 1;
	//}

	std::cout << ok << "Done, exiting..." << std::endl;
	return 0;
}