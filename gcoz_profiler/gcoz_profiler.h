#pragma once

#include <Windows.h>
#include <iostream>
#include <fileapi.h>
#include "../shared/Messages.h"
#include "Injector.h"
#include "Communication.h"
#include "ProfilerStatusManager.h"
#include "MessageHandler.h"
#include <spdlog/spdlog.h>

const char* ok = "[+] ";
const char* err = "[-] ";
const char* inf = "[*] ";