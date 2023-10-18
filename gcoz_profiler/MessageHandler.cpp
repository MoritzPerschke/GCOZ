#include "MessageHandler.h"

MessageHandler::MessageHandler(string& _processName){
	resH = ResultsHandler(_processName);
}

ProfilerStatus MessageHandler::nextStatus() {
	/*
	- are all baseline measuremetns collected? (should one be one)
	- are all slowdowns applied to all methods?
	- are all slowdowns applied to every method?
	- (are all threadIDs collected?)
	- is everything done?
	*/
	if (!calc.isBaselineAdded()) {
		return ProfilerStatus::GCOZ_MEASURE;
	}
	else if (!idsC.isDone()) {
		return ProfilerStatus::GCOZ_COLLECT_THREAD_IDS;
	}
	else if (!calc.dataCollectedAllMethods()){
		return ProfilerStatus::GCOZ_PROFILE;
	}
	else if (!calc.dataCollected()) {
		return ProfilerStatus::GCOZ_PROFILE;
	}
	else {
		return ProfilerStatus::GCOZ_FINISH;
	}
}

/* GCOZ_MEASURE: write/return nothing, baseline method durations are needed
/* GCOZ_COLLECT_THREAD_IDS: return the next method to collect Thread IDs from as int
/* GCOZ_PROFILE: writes the required delays to message given as input
/* GCOZ_WAIT/FINISH: return nothing, done/fail */ //ugly but should work
void MessageHandler::nextMessage(ProfilerStatus _status, ProfilerMessage& _msg, int& _method) {
	switch (_status) {
	case ProfilerStatus::GCOZ_MEASURE:
		break;
	case ProfilerStatus::GCOZ_COLLECT_THREAD_IDS:
		_method = idsC.nextMethod();
		break;
	case ProfilerStatus::GCOZ_PROFILE:
		calc.calculateDelays(lastSpeedup, lastMethod, _msg.delays);
		_method = lastMethod;
		_msg.valid = true;
		break;
	case ProfilerStatus::GCOZ_WAIT:
		// shouldn't happen
		break;
	case ProfilerStatus::GCOZ_FINISH:
		// shouldn't need a message
		break;
	}
}

void MessageHandler::finish() {
	idsC.finish(resH);
	resH.exportResults();
}

void MessageHandler::handleThreadIDs(ThreadIDMessage _ids){
	idsC.addIDs(_ids.threadIDs);
}

void MessageHandler::handleMeasurement(Measurement _measurement){
	resH.addBaseline(_measurement.frameTimes, _measurement.durations, _measurement.methodCalls);
	calc.addBaseline(_measurement.frameTimes, _measurement.durations, _measurement.methodCalls);
}

void MessageHandler::handleResult(Result _result){
	if (!calc.dataCollectedAllMethods()) {
		resH.addResultAll(_result.frameTimes, _result.frameRates, lastSpeedup);
		calc.measurementDoneAll(lastSpeedup);
	}
	else {
		resH.addResultSingle(_result.frameTimes, _result.frameRates, lastMethod, lastSpeedup);
		calc.measurementDoneSingle(lastSpeedup, lastMethod);
	}
}