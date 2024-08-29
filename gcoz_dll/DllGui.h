/*
* This was an attempt to create a GUI providing Information about 
* the current profiling run within the games window.
* There was a bit of a hit to performance but not in a very impactful way.
* The reason this is not used anymore is that it required too much effort
* to maintain during the last sprint of the thesis
*/
#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <chrono>

#include"../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_win32.h"
#include "../external/imgui/imgui_impl_dx11.h"

#include "ProfilerStatusManager.h"

static WNDPROC oWndProc = NULL;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// this could also just be an object
namespace GUI {

	void init(IDXGISwapChain* pSwapChain);
	void showGCOZgui(ProfilerStatusManager _man, std::string _debugMsg);

}