#pragma once
#include <chrono>
#include <array>
	
using Duration = std::chrono::duration<double>;

enum Command {
	DLL_DELAY,
	DLL_FLUSH
};

struct Message {
	bool valid;
};

struct ProfilerMessage : Message {
	std::array<DWORD, 205> delays;
	Command command;
};

struct DllMessage : Message {
	std::array<std::chrono::nanoseconds, 205> durations;
};
