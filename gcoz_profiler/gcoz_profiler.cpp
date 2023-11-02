// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "gcoz_profiler.h"

int main(int argc, char* argv[]) {

	/* Logging */
	spdlog::set_pattern("[%R][%^%l%$] %v");

	if (argc < 3) {
		spdlog::error("Usage: .\\gcoz_profiler.exe </data subdirectory> <PID>");
		spdlog::info("Sizes:\n\t\tProfilerStatus: {}\n\t\tMethod: {}", sizeof(ProfilerStatus), sizeof(int));
		return 1;
	}
	DWORD PID = atoi(argv[2]);
	string processName = string(argv[1]);

	/* Communication */
	Communication com;
	com.init();

	ProfilerStatusManager statusManager;
	statusManager.setStatus(ProfilerStatus::GCOZ_WAIT);

	/* Injection */
	// this HAS TO happen after Communication AND ProfilerStatusManager is initialized
	spdlog::info("waiting for injection");
	system("pause");
	Injector injector = Injector();
	injector.inject_dll(PID);

	/* Main profiling loop */
	spdlog::info("Waiting for game to be in steady state");
	system("pause");
	spdlog::info("Waiting 5 more seconds");
	std::this_thread::sleep_for(std::chrono::seconds(5)); // wait to get into steady state and wait to tab back into game

	spdlog::info("Starting profiling...");
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
			spdlog::info("Next Status: {}", profilerStatusString(nextStatus));
			lastStatus = nextStatus;
		}
		if (nextStatus == ProfilerStatus::GCOZ_FINISH) {
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
				spdlog::info("Received Measurement");
				break;

			case ProfilerStatus::GCOZ_PROFILE:
				// all methods slowed results ? choose first method : choose next slowdown
				Result result = com.getResult();
				messageHandler.handleResult(result);
				spdlog::info("Received Results");
			break;

			 case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS:
				 ThreadIDMessage ids = com.getThreadIDs();
				 messageHandler.handleThreadIDs(ids);
				 break;
			case ProfilerStatus::GCOZ_WAIT:
				// should not happen?
				spdlog::error("Received message during WAIT");
				break;
					
			case ProfilerStatus::GCOZ_FINISH:
				// end
				spdlog::error("Received message during FINISH");
				break;
			default:
				break;
			}
		}
	} while(!profilingDone);
	spdlog::info("All Data collected, saving...");
	messageHandler.finish();

	spdlog::info("Done, exiting...");
	return 0;
}	