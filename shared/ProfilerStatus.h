#pragma once
#ifndef _PROFILER_STATUS
#define _PROFILER_STATUS

#define X_PROFILER_STATUS\
	X(GCOZ_MEASURE)\
	X(GCOZ_PROFILE)\
	X(GCOZ_COLLECT_THREAD_IDS)\
	X(GCOZ_WAIT)\
	X(GCOZ_FINISH)\
	X(GCOZ_FAIL)

enum ProfilerStatus {
#define X(_STATUS)\
	_STATUS,
	X_PROFILER_STATUS
#undef X
};

/// TODO: probably not great to inline these but i just don't want to deal with this rn
static inline std::string profilerStatusString(ProfilerStatus _status) {
	switch (_status) {
#define X(_STATUS)\
	case _STATUS:\
		return #_STATUS;\
		break;
		X_PROFILER_STATUS
#undef X
	default:
		return "Failure in status conversion";
	}
}

static inline std::wstring profilerStatusWstring(ProfilerStatus _status) {
	std::string narrowStatus;
	switch (_status) {
#define X(_STATUS)\
	case _STATUS:\
		narrowStatus = #_STATUS;\
		return std::wstring(narrowStatus.begin(), narrowStatus.end());\
		break;
		X_PROFILER_STATUS
#undef X
	return L"Failure in status conversion";
	}
}

#endif // !_PROFILER_STATUS
