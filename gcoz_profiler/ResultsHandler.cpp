#include "ResultsHandler.h"

string uniqueFileName(fs::path& _dir, string _base, string _ext = ".json") {
	std::string unique = _base + _ext;
	int counter = 0;
	do {
		std::ostringstream oss;
		oss << _base << "_" << counter << _ext;
		unique = oss.str();
		counter++;
	}while (fs::exists(_dir / unique));

	return unique;
}

json ResultsHandler::getMethodDurations(){
	managed_shared_memory segment(open_only, "gcoz_SharedMemory");
	IPC::DurationVector_Map* methodDurationsMap = segment.find<IPC::DurationVector_Map>("Durations_Map").first;

	named_mutex durationsMutex(open_only, "gcoz_Durations_Map_Mutex");
	scoped_lock<named_mutex> lock(durationsMutex);

	json methodDurations;
	for (const auto& method : *methodDurationsMap) { // map is method->Durations(vector)
		// first  = method
		// second = durations
		string locM = methodNames[method.first];
		for (const auto& duration : method.second) 
		{ // loop all measured durations
			methodDurations[locM].push_back(duration);
		}
	}
	spdlog::info("Measured durations of method added to file");
	return methodDurations;
}

json ResultsHandler::getFrameRates(){

	named_mutex ratesMutex(open_only, "gcoz_FrameRates_Map_Mutex");
	scoped_lock<named_mutex> lock(ratesMutex);

	managed_shared_memory segment(open_only, "gcoz_SharedMemory");
	IPC::ResultsMap_Map* frameRatesMap = segment.find<IPC::ResultsMap_Map>("FrameRates_Map").first;
	
	json frameRates;
	for (const auto& elem : *frameRatesMap) {
		// first  = method
		// second = delay->times
		string method = elem.first == -1 ? "All" : methodNames[elem.first];
		for (const auto& delay : elem.second)
		{ // loop over all the different delays
			string locD = to_string(static_cast<int>(delay.first * 10)) + "%";
			for (const auto& time : delay.second)
			{ // iterate the vector of frametimes
				float t = delay.first;
				frameRates[method][locD].push_back(time);
			}
		}
	}

	return frameRates;
}

json ResultsHandler::getFrameTimes() {
	named_mutex timesMutex(open_only, "gcoz_FrameTimes_Map_Mutex");
	scoped_lock<named_mutex> lock(timesMutex);

	managed_shared_memory segment(open_only, "gcoz_SharedMemory");
	IPC::ResultsMap_Map* frameTimesMap = segment.find<IPC::ResultsMap_Map>("FrameTimes_Map").first;

	json frameTimes;
	for (const auto& elem : *frameTimesMap) {
		// first  = method
		// second = delay->times
		string method = elem.first == -1 ? "All" : methodNames[elem.first];
		for (const auto& delay : elem.second)
		{ // loop over all the different delays
			string locD = to_string(static_cast<int>(delay.first * 10)) + "%";
			for (const auto& time : delay.second)
			{ // iterate the vector of frametimes
				frameTimes[method][locD].push_back(time);
			}
		}
	}

	return frameTimes;
}

json ResultsHandler::getThreadIDs(){
	named_mutex threadsMutex(open_only, "gcoz_ThreadID_Map_Mutex");
	scoped_lock<named_mutex> lock(threadsMutex);
	
	managed_shared_memory segment(open_only, "gcoz_SharedMemory");
	IPC::ThreadIdVector_Map* threadIDmap = segment.find<IPC::ThreadIdVector_Map>("ThreadID_Map").first;

	json id_vector;
	for (const auto& idMapElem: *threadIDmap) {
		//first  = methodID
		//second = ThreadIDs
		string locM = methodNames[idMapElem.first];
		for (const auto& id : idMapElem.second) {
			id_vector[locM].push_back(id);
		}
	}
	return id_vector;
}

fs::path cut(fs::path _path) {
	return _path.lexically_relative("../../..");
}

void ResultsHandler::exportResults(string _outputName){

	/*-------[Build necessary filepaths to save all the files]-------*/
	// create data dir if it does not exist
	fs::path targetPath = "../../../data";
	fs::create_directories(targetPath);
	spdlog::info("Folder /data found");

	// create folder for game if it does not exist
	fs::path gamePath = targetPath / _outputName;
	fs::create_directories(gamePath);

	// create folder for run
	fs::path folderPath = gamePath / uniqueFileName(gamePath, "run", "");
	fs::create_directories(folderPath);

	// build path for files
	fs::path timesPath		= folderPath / "Frametimes.json";
	fs::path ratesPath		= folderPath / "Framerates.json";
	fs::path durationsPath  = folderPath / "MethodDurations.json";
	fs::path idsPath		= folderPath / "ThreadIDs.json";

	spdlog::info("Writing Data:");
	spdlog::info("|_ {}", cut(folderPath).string());
	spdlog::info("    |_ {}", "Framerates.json");
	spdlog::info("    |_ {}", "Frametimes.json");
	spdlog::info("    |_ {}", "MethodDurations.json");
	spdlog::info("    |_ {}", "ThreadIDs.json");

	/*------------------------[Get the Data]-------------------------*/
	json frameTimes = getFrameTimes();
	json frameRates = getFrameRates();
	json durations  = getMethodDurations();
	json threadIDs  = getThreadIDs();

	/*-----------------------[Write the Data]------------------------*/
	std::ofstream outputFile;
	outputFile.open(timesPath.string(), std::ios::out | std::ios::trunc);
	outputFile << frameTimes.dump();
	outputFile.close();

	outputFile.open(ratesPath.string(), std::ios::out | std::ios::trunc);
	outputFile << frameRates.dump();
	outputFile.close();

	outputFile.open(durationsPath.string(), std::ios::out | std::ios::trunc);
	outputFile << durations.dump();
	outputFile.close();

	outputFile.open(idsPath.string(), std::ios::out | std::ios::trunc);
	outputFile << threadIDs.dump();
	outputFile.close();

	spdlog::info("Done writing data");
}
