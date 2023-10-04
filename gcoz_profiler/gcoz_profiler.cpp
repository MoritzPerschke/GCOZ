// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "gcoz_profiler.h"

int main(int argc, char* argv[]) {

	// using switch here is dumb
	if (argc < 3) {
		std::cout << err << "Usage: .\\gcoz_profiler.exe </data subdirectory> <PID>";
		return 1;
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

		std::cout << ok << "Starting profiling" << std::endl;
		ProfilerStatusManager statusManager = ProfilerStatusManager(processName);
		MessageHandler messageHandler;
		bool profilingDone = false;
		do {

			DWORD waitResult = com.waitMsg();
			if (waitResult == WAIT_OBJECT_0) {
				switch (statusManager.getPreviousStatus()) {
				case ProfilerStatus::GCOZ_MEASURE:
					Measurement measurement = static_cast<Measurement>(com.getMessage(statusManager.getPreviousStatus()));
					// add baseline stuff and next method
					messageHandler.handleMeasurement(measurement);
					break;

				case ProfilerStatus::GCOZ_PROFILE:
					// all methods slowed results ? choose first method : choose next slowdown
					Result result = static_cast<Result>(com.getMessage(statusManager.getPreviousStatus()));
					messageHandler.handleResult(result);
					break;

				// case ProfilerStatus::GCOZ_COLLECT_THREADID:
				case ProfilerStatus::GCOZ_WAIT:
					// should not happen?
					break;
					
				case ProfilerStatus::GCOZ_FINISH:
					// end
					break;
				}
			}
			/*
			- calc next status
			- build new message
			- send new message
			- wait for message recv event
			- set new status
			*/
			ProfilerStatus nextStatus = messageHandler.nextStatus();
			profilingDone = (nextStatus == ProfilerStatus::GCOZ_FINISH);
			ProfilerMessage nextMsg = {};
			messageHandler.nextMessage(nextMsg, nextStatus);
			com.sendMessage(nextMsg);
			com.waitRecv();
			statusManager.setStatus(nextStatus);
			statusManager.announceStatusChange();

		} while(!profilingDone);
		messageHandler.finish();

	std::cout << ok << "Done, exiting..." << std::endl;
	return 0;
}	