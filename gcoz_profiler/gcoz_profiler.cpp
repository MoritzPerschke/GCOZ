// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "gcoz_profiler.h"

int main(int argc, char* argv[]) {

	// using switch here is dumb
	if (argc < 3) {
		std::cout << err << "Usage: .\\gcoz_profiler.exe <Name of Game> <PID>";
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
	char profile;
	do{
		std::cout << inf << "Waiting for game to be in steady state" << std::endl;
		system("pause");
		std::cout << inf << "Waiting 10 more seconds" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(10)); // wait to get into steady state and wait 10 to tab back into game

		ProfilerStatusManager man = ProfilerStatusManager(processName);
		while (!man.dataCollected()) {
			std::cout << ok << "Starting profiling" << std::endl;
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
		}
		man.~ProfilerStatusManager();
		std::cout << ok << "Run done, go again? Y/N" << std::endl;
		std::cin >> profile;
	} while (!std::cin.fail() && (profile == 'Y' || profile == 'y'));

	std::cout << ok << "Done, exiting..." << std::endl;
	return 0;
}