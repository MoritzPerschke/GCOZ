#pragma once
#include <chrono>

typedef enum Command {
	DLL_DELAY,
	DLL_FLUSH
};

typedef struct Message {
	int delay[205];
	Command command;
	bool valid;
};

typedef struct Response {
	std::chrono::high_resolution_clock::time_point timestamp;
};
