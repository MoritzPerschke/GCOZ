#pragma once

#include <Windows.h>
#include <iostream>
#include <fileapi.h>
#include <Messages.h>
#include "ProfilerStatusManager.h"
#include <spdlog/spdlog.h>
#include "Communication.h"
#include "Injector.h"

const char* ok = "[+] ";
const char* err = "[-] ";
const char* inf = "[*] ";