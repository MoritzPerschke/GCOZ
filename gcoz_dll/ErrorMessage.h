#pragma once

#include <Windows.h>
#include <string>

// taken from https://stackoverflow.com/a/17387176
/* This function calls GetLastError() and 
   uses FormatMessageW() to turn the DWORD into the corresponding
   Error Message. */
std::wstring GetLastErrorAsString();

/* This function uses GetLastErrorAsString to Display a MessageBox
   containing the error message and an error icon */
int DisplayErrorBox(std::wstring _location);

/* This function uses the given wstring to display a MessageBox
   with the erorr icon */
int DisplayErrorBox(std::wstring _title, std::wstring _body);

/* This function displays a MessageBox with the given title 
   and body and the lowercase 'i' icon */
int DisplayInfoBox(std::wstring _title, std::wstring _body);