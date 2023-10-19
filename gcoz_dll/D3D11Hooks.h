#pragma once
#include <Windows.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <thread>
#include <tchar.h>
#include <dwmapi.h>

#include "ErrorMessage.h"
#include "HelperMacros.h"
#include "DelayManager.h"
#include "MethodDurations.h"
#include "ThreadIDs.h"
#include "ProfilerStatusManager.h"
#include "../gcoz_profiler/Constants.h"

#include"../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_win32.h"
#include "../external/imgui/imgui_impl_dx11.h"

static WNDPROC oWndProc = NULL;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK hkWindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) {
		return true;
	}
	return ::CallWindowProcA(oWndProc, hwnd, uMsg, wParam, lParam);
}

	DelayManager delays;
	ProfilerStatusManager man;
	Communication com;

	void little_sleep(Nanoseconds _delay) { // https://stackoverflow.com/a/45571538
		auto start = MethodDurations::now();
		auto end = start + _delay;
		while (MethodDurations::now() < end) {
			if ((MethodDurations::now() - end) > std::chrono::milliseconds(1)) {
				std::this_thread::yield();
			}
		}
	}

	/* expands to function pointers for D3D11 methods */
#define X(idx, returnType, name, ...) typedef returnType (__stdcall* name)(PARAMETER_TYPES(__VA_ARGS__));
	D3D11_METHODS
		D3D11_METHODS_VOID
#undef X

		/* expands to setting previously defined pointers to NULL */
#define X(idx, returnType, name, ...) static name o##name = NULL;
		D3D11_METHODS
		D3D11_METHODS_VOID
#undef X

		/* expands to hooked d3d11 function */
		// maybe always measure duration to verify pauses
#define X(_IDX, _RETURN_TYPE, _NAME, ...) \
		_RETURN_TYPE __stdcall hk##_NAME(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
			_RETURN_TYPE value; \
			MethodDurations::Timepoint start; \
			switch (man.getStatus()) { \
				case ProfilerStatus::GCOZ_MEASURE : \
					start = MethodDurations::now(); \
					value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
					MethodDurations::Duration duration = MethodDurations::now() - start; \
					MethodDurations::addDuration(_IDX, duration); \
					break; \
				case ProfilerStatus::GCOZ_PROFILE : \
					value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
					little_sleep(delays.getDelay(_IDX)); \
					break; \
				case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS: \
					value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
					ThreadIDs::mutex.lock();\
					ThreadIDs::addID(_IDX, std::this_thread::get_id());\
					ThreadIDs::mutex.unlock(); \
				case ProfilerStatus::GCOZ_WAIT : \
					value = o##_NAME(PARAMETER_NAMES(__VA_ARGS__));	\
					break; \
			} \
			return value; \
		}
		D3D11_METHODS
#undef X

		/* same as above just for void functions (no return)*/
#define X(_IDX, _RETURN_TYPE, _NAME, ...) \
		_RETURN_TYPE __stdcall hk##_NAME(FUNCTION_SIGNATURE(__VA_ARGS__)){ \
			MethodDurations::Timepoint start; \
			switch (man.getStatus()) { \
				case ProfilerStatus::GCOZ_MEASURE : \
					start = MethodDurations::now(); \
					o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
					MethodDurations::Duration duration = MethodDurations::now() - start; \
					MethodDurations::addDuration(_IDX, duration); \
					break; \
				case ProfilerStatus::GCOZ_PROFILE : \
					o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
					little_sleep(delays.getDelay(_IDX)); \
					break; \
				case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS: \
					o##_NAME(PARAMETER_NAMES(__VA_ARGS__)); \
					ThreadIDs::mutex.lock();\
					ThreadIDs::addID(_IDX, std::this_thread::get_id());\
					ThreadIDs::mutex.unlock(); \
				case ProfilerStatus::GCOZ_WAIT : \
					o##_NAME(PARAMETER_NAMES(__VA_ARGS__));	\
					break; \
			} \
		}
		D3D11_METHODS_VOID
#undef X

	typedef HRESULT(__stdcall* Present)(IDXGISwapChain*, UINT, UINT);
	static Present oPresent = NULL;
	HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
		static unsigned long long int callCount = 0;
		MethodDurations::Timepoint start;

		switch (man.getStatus()) {
		case ProfilerStatus::GCOZ_MEASURE: // measure times of D3D11 Methods, nothing else
			MethodDurations::presentStart();
			if (callCount++ == MEASURE_FRAME_COUNT) { // this could prob be done in seperate thread
				callCount = 0;
				Measurement send = {};
				// remove measurement of frametimes, use allMethods[0%] as baseline
				send.frameTimes = MethodDurations::getPresentTimes();
				//send.frameRates = MethodDurations::getFrameRates();
				send.durations = MethodDurations::getDurations();
				send.methodCalls = MethodDurations::getCallAmounts();
				send.valid = true;
				com.sendMeasurement(send);
				delays.resetDelays();
				man.setStatus(ProfilerStatus::GCOZ_WAIT);
				man.announceStatusChange();
			}
			MethodDurations::presentEnd();
			break;

		case ProfilerStatus::GCOZ_PROFILE: // apply last received delays and measure FPS
			MethodDurations::presentStart();
			if (callCount++ == MEASURE_FRAME_COUNT) {
				callCount = 0;
				Result send = {};
				send.frameTimes = MethodDurations::getPresentTimes();
				send.frameRates = MethodDurations::getFrameRates();
				send.valid = true;
				com.sendResult(send);
				delays.resetDelays();
				man.setStatus(ProfilerStatus::GCOZ_WAIT);
				man.announceStatusChange();
			}
			MethodDurations::presentEnd();
			break;

		case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS:
			MethodDurations::presentStart();
			if (callCount++ == METHOD_THREAD_COLLECTION_FRAME_COUNT) {
				callCount = 0;
				ThreadIDMessage ids = {};
				ThreadIDs::mutex.lock();
				ids.threadIDs = ThreadIDs::getIDs(man.getMethod());
				ThreadIDs::mutex.unlock();
				ids.valid = true;
				com.sendThreadIDs(ids);
				man.setStatus(ProfilerStatus::GCOZ_WAIT);
				man.announceStatusChange();
			}
			MethodDurations::presentEnd();
			break;

		case ProfilerStatus::GCOZ_WAIT: // do nothing, wait for message from Profiler
			if (com.newDataAvailable()) {
				ProfilerMessage newData = com.getMessage();
				if (newData.valid) {
					man.waitNewStatus();
					if (man.getStatus() == ProfilerStatus::GCOZ_MEASURE) {
						try {
							delays.updateDelays(newData.delays);
						}
						catch (...) {
							DisplayErrorBox(L"Dll Main");
						}
					}
				}
			}
			break;
		case ProfilerStatus::GCOZ_FINISH:
			//DisplayInfoBox(L"Dll Main", L"finish case in Present switch");
		default:
			//DisplayInfoBox(L"Dll Main", L"default case in Present switch");
			break;
		} // switch(ProfilerStatusManager::currentStatus)

		return oPresent(pSwapChain, SyncInterval, Flags);
	}

	long __stdcall hkPresent1(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		static bool init = false;

		if (!init)
		{
			DXGI_SWAP_CHAIN_DESC desc;
			pSwapChain->GetDesc(&desc);

			ID3D11Device* device;
			pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device);

			ID3D11DeviceContext* context;
			device->GetImmediateContext(&context);
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

			oWndProc = (WNDPROC) ::SetWindowLongPtr(desc.OutputWindow, GWLP_WNDPROC, (LONG_PTR)hkWindowProc); // changed to (LONG_PTR) from example

			ImGui_ImplWin32_Init(desc.OutputWindow);
			ImGui_ImplDX11_Init(device, context);

			init = true;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
		ImGui::Begin("Hello");
		ImGui::Text("Overlay here");
		ImGui::Button("do nothing");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return oPresent(pSwapChain, SyncInterval, Flags);
	}


	void hookD3D11() {
		#define X(idx, returnType, name, ...)\
				kiero::bind(idx, (void**)&o##name, hk##name);
				D3D11_METHODS
				D3D11_METHODS_VOID
		#undef X
		kiero::bind(8, (void**)&oPresent, hkPresent1);
		DisplayInfoBox(L"Progress", L"D3D11 functions hooked");
	}

