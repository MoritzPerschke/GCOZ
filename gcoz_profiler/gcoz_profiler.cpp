// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "gcoz_profiler.h"

int main(int argc, char* argv[]) {

	// using switch here is dumb
	if (argc < 3) {
		std::cout << inf << "Sizes:\n\tProfilerStatus: " << sizeof(ProfilerStatus) << "\n\tMethod: " << sizeof(int) << std::endl;
		std::cout << err << "Usage: .\\gcoz_profiler.exe </data subdirectory> <PID>";
		return 1;
	}
	DWORD PID = atoi(argv[2]);
	string processName = string(argv[1]);

	//try {
		/* Communication */
		Communication com;
		com.init();

	ProfilerStatusManager statusManager;
	statusManager.setStatus(ProfilerStatus::GCOZ_WAIT);

	/* Injection */
	// this HAS TO happen after Communication AND ProfilerStatusManager is initialized
	Injector injector = Injector();
	injector.inject_dll(PID);

	/* Main profiling loop */
	std::cout << inf << "Waiting for game to be in steady state" << std::endl;
	system("pause");
	std::cout << inf << "Waiting 10 more seconds" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(10)); // wait to get into steady state and wait 10 to tab back into game

	std::cout << ok << "Starting profiling" << std::endl;
	MessageHandler messageHandler = MessageHandler(processName);
	static ProfilerStatus lastStatus = statusManager.getCurrentStatus();
	bool profilingDone = false;
	do {/*
		* - calc next status
		* - build new message
		* - send new message
		* - wait for message recv event
		* - set new status           */
		ProfilerStatus nextStatus = messageHandler.nextStatus();
		if (lastStatus != nextStatus) {
			std::cout << ok << "Next Status: " << profilerStatusString(nextStatus) << std::endl;
		}
		profilingDone = (nextStatus == ProfilerStatus::GCOZ_FINISH);
		if (nextStatus == ProfilerStatus::GCOZ_PROFILE) { // change this back to finish state
			profilingDone = true;
			break;
		}
		ProfilerMessage nextMsg = {};
		int nextMethod = -1;
		messageHandler.nextMessage(nextStatus, nextMsg, nextMethod);
		statusManager.setCurrentMethod(nextMethod);
		com.sendMessage(nextMsg);
		com.waitRecv();
		statusManager.setStatus(nextStatus);
		statusManager.announceStatusChange();

		/*
		* - wait for response from game
		* - either add measurement or result
		*/
		DWORD waitResult = com.waitMsg();
		if (waitResult == WAIT_OBJECT_0) {
			switch (nextStatus) {
			case ProfilerStatus::GCOZ_MEASURE:
				Measurement measurement = com.getMeasurement();
				// add baseline stuff and next method
				messageHandler.handleMeasurement(measurement);
				std::cout << ok << "Received Measurement" << std::endl;
				break;

			case ProfilerStatus::GCOZ_PROFILE:
				// all methods slowed results ? choose first method : choose next slowdown
				Result result = com.getResult();
				messageHandler.handleResult(result);
				std::cout << ok << "Received Results" << std::endl;
			break;

			 case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS:
				 ThreadIDMessage ids = com.getThreadIDs();
				 messageHandler.handleThreadIDs(ids);
				 break;
			case ProfilerStatus::GCOZ_WAIT:
				// should not happen?
				std::cout << err << "Received message during WAIT" << std::endl;
				break;
					
			case ProfilerStatus::GCOZ_FINISH:
				// end
				std::cout << err << "Received message during FINISH" << std::endl;
				break;
			default:
				break;
			}
		}
	} while(!profilingDone);
	std::cout << ok << "All Data collected, saving..." << std::endl;
	messageHandler.finish();

	std::cout << ok << "Done, exiting..." << std::endl;
	return 0;
}	