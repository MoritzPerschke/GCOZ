#include "ErrorMessage.h"

std::wstring GetLastErrorAsString()
{
	DWORD errorCode = GetLastError();
	LPWSTR messageBuffer = nullptr;
	size_t size = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&messageBuffer,
		0,
		NULL
	);

	std::wstring message(messageBuffer, size);

	LocalFree(messageBuffer);

	return message;
}

int DisplayErrorBox(std::wstring _location) {
	return MessageBoxW(
		NULL,
		GetLastErrorAsString().c_str(),
		_location.c_str(),
		MB_ICONERROR | MB_SETFOREGROUND | MB_OK
	);
}

int DisplayErrorBox(std::wstring _title, std::wstring _body) {
	return MessageBoxW(
		NULL,
		_body.c_str(),
		_title.c_str(),
		MB_ICONERROR | MB_SETFOREGROUND | MB_OK
	);
}

int DisplayInfoBox(std::wstring _title, std::wstring _body) {
	return MessageBoxW(
		NULL,
		_body.c_str(),
		_title.c_str(),
		MB_ICONINFORMATION | MB_SETFOREGROUND | MB_OK
	);
}