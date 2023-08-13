#pragma once

#include <Windows.h>
#include "../gcoz_profiler/ProfilerStatus.h"

namespace ProfilerStatusManager {
	static ProfilerStatus currentStatus = ProfilerStatus::GCOZ_MEASURE;

	void changeStatus(ProfilerStatus _newStatus) {
		currentStatus = _newStatus;
	}
}