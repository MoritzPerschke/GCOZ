#pragma once

#include <Windows.h>
#include <string>

// taken from https://stackoverflow.com/a/17387176
/* This function calls GetLastError() and 
   uses FormatMessageW() to turn the DWORD into the corresponding
   Error Message. The return value is LPCWSTR */
std::wstring GetLastErrorAsString();

/* This function uses GetLastErrorAsString to Display a MessageBox
   containing the error message */
int DisplayErrorBox(std::wstring _location);