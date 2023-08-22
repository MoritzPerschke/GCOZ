#pragma once

#ifndef _CONSTANT_VALUES
#define _CONSTANT_VALUES
#include <chrono>

#define D3D11_METHOD_COUNT 205
#define MEASURE_FRAME_COUNT 500 // need the same fc for measuring/profiling since switching to array

using Nanoseconds = std::chrono::nanoseconds;

typedef std::array<Nanoseconds, MEASURE_FRAME_COUNT> frametimeArray;
typedef std::array<Nanoseconds, D3D11_METHOD_COUNT> durationArray;
typedef std::array<DWORD, D3D11_METHOD_COUNT> delayArray;


#endif // !_CONSTANT_VALUES
