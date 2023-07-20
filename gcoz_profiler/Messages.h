#pragma once
#include <chrono>
#include <array>

typedef enum Command {
	DLL_DELAY,
	DLL_FLUSH
};

typedef struct Message {
	bool valid;
};

typedef struct ProfilerMessage : Message {
	std::array<DWORD, 205> delays;
	Command command;
};

typedef struct DllMessage : Message {
	std::chrono::high_resolution_clock::time_point timestamp;
};
