#pragma once
#include <Windows.h>
#include <iostream>
#include <cstring>
#include <spdlog/spdlog.h>

#include "../shared/status.h"

/* Class providing the functionality to inject the dll into
*  a process with the given PID
*/
class Injector
{
private:
	// This needs to be a const wchar_t array, and I am not sure
	// whether it is possible to convert some dynamic path into this
	const wchar_t dllPath[82] = L"C:\\Users\\Moritz\\Documents\\bachelor_thesis\\github\\gcoz\\gcoz\\x64\\Debug\\gcoz_dll.dll";

public:
	// Create a remote thread in the given process that
	// uses LoadLibraryW to inject the dll
	int inject_dll(const DWORD _PID);
};

