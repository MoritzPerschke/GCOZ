#pragma once
#include <Windows.h>
#include <iostream>

#include "status.h"

class Injector
{
private:
	wchar_t* dllPath;

public:
	Injector(wchar_t* dllPath)
		: dllPath(dllPath){}

	int inject_dll(const DWORD _PID);
};

