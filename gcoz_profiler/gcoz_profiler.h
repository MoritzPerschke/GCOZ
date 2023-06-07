#pragma once

#include <Windows.h>
#include <iostream>
#include <fileapi.h>

#include "Messages.h"

const char* k = "[+] ";
const char* e = "[-] ";
const char* i = "[*] ";

HANDLE hFileMapping;
LPVOID pSharedMemory;
Message* pData;
Message dllMessage;
HANDLE hDataWrittenEvent;