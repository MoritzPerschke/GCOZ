// gcoz_profiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "gcoz_profiler.h"

int main(int argc, char* argv[]) {

	/* Logging */
	spdlog::set_pattern("[%R][%^%l%$] %v");

	if (argc < 3) {
		spdlog::error("Usage: .\\gcoz_profiler.exe </data subdirectory> <PID>");
		return 1;
	}
	string processName = string(argv[1]);
	DWORD PID = atoi(argv[2]);

	/* Communication */
	Communication com;

	ProfilerStatusManager statusManager(processName);
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
	static ProfilerStatus lastStatus = statusManager.getCurrentStatus();
	bool profilingDone = false;

	do {/*
		* - calc next status
		* - build new message
		* - send new message
		* - wait for dll to be done
	    */
		ProfilerStatus nextStatus = statusManager.getNextStatus();
		if (nextStatus!= lastStatus) {
			spdlog::info("Next Status: {}", profilerStatusString(nextStatus));
			lastStatus = nextStatus;
		}
		if (nextStatus == ProfilerStatus::GCOZ_FINISH) {
			profilingDone = true;
			break;
		}

		ProfilerMessage nextMsg = {};
		statusManager.nextMessage(nextStatus, nextMsg);
		nextMsg.valid = true;
		com.sendMessage(nextMsg);
		com.waitRecv();
		statusManager.setStatus(nextStatus);
		statusManager.announceStatusChange();

		DWORD waitResult = com.waitDllDone();
	} while(!profilingDone);

	spdlog::info("All Data collected, saving...");
	statusManager.finish(processName);

	spdlog::info("Done, exiting...");
	return 0;
}	