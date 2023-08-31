#pragma once

#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <vector>
#include <numeric>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <Psapi.h>


// high_resolution_clock on windows just wraps the steady_clock anyway
using Nanoseconds = std::chrono::nanoseconds;
using Microseconds = std::chrono::microseconds;
namespace fs = std::filesystem;
using json = nlohmann::json;

const char* ok = "[+] ";
const char* inf = "[*] ";
const char* err = "[-] ";

std::chrono::steady_clock timer;

typedef long long (*sleep_function)(DWORD);

long long little_sleep_ns(DWORD _duration);
long long little_sleep_us(DWORD _duration);