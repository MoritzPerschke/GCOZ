#pragma once
#include <Windows.h>
#include <iostream>
#include <cstring>
#include <spdlog/spdlog.h>

#include "../shared/status.h"

class Injector
{
private:
	const wchar_t dllPath[82] = L"C:\\Users\\Moritz\\Documents\\bachelor_thesis\\github\\gcoz\\gcoz\\x64\\Debug\\gcoz_dll.dll";

public:
	int inject_dll(const DWORD _PID);
};

