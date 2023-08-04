#include "D3D11Hooks.h"

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	MethodDurations::presentCalled();

	static unsigned long long int callCount = 0;
	callCount++;

	std::this_thread::sleep_for(std::chrono::milliseconds(delays.getDelay(8))); // maybe make this more readable ('using')

	MethodDurations::Timepoint start = MethodDurations::now();
	HRESULT value = oPresent(pSwapChain, SyncInterval, Flags);
	MethodDurations::Duration duration = MethodDurations::now() - start;
	MethodDurations::addDuration(8, duration);

	if (callCount % 500 == 0) {
		// update the profiler with the average execution times
		DllMessage send = {};
		if (MethodDurations::getPresentTimes(send) == 0) { // this does not take, profiler still gets 0-length vector
			DisplayErrorBox(L"Sending Message", L"frameTimePoints vector is empty");
		}
		send.durations = MethodDurations::getDurations();
		send.valid = true;
		if (!com.sendMessage(send)) {
			DisplayErrorBox(L"Sending Message to Profiler");
		}

		// update the delays (maybe this should happen in Delaymanager Class)
		ProfilerMessage newDelays = com.getMessage(0);
		if (newDelays.valid) {
			delays.updateDelays(newDelays.delays);
		}
		else {
			//DisplayErrorBox(L"Updating Delays", L"Failed to get updated delays from profiler");
		}
	}

	return value;
}

void hookD3D11() {
	kiero::bind(8, (void**)&oPresent, hkPresent);
	#define X(idx, returnType, name, ...)\
			kiero::bind(idx, (void**)&o##name, hk##name);
		D3D11_METHODS
		D3D11_METHODS_VOID
	#undef X
	DisplayInfoBox(L"Progress", L"D3D11 functions hooked");
}

