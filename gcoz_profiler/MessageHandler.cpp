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

void MessageHandler::nextMessage(ProfilerMessage& _msg, ProfilerStatus _status) {
	switch (_status) {
	case ProfilerStatus::GCOZ_MEASURE:
		break;
	case ProfilerStatus::GCOZ_PROFILE:
		calc.calculateDelays(lastSpeedup, lastMethod, _msg.delays);
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
	resH.exportResults();
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