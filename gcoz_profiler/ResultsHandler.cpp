#include "ResultsHandler.h"

string uniqueFileName(fs::path& _dir, string _base) {
	string ext = ".json";
	std::string unique = _base + ext;
	int counter = 0;

	do {
		std::ostringstream oss;
		oss << _base << "_" << counter << ext;
		unique = oss.str();
		counter++;
	}while (fs::exists(_dir / unique));

	return unique;
}

ResultsHandler::ResultsHandler() {
	spdlog::warn("Resultshandler Constructor called without processName");
}

ResultsHandler::ResultsHandler(string& _processName){
	// create data dir if it does not exist
	fs::path targetPath = "../../../data";
	fs::create_directories(targetPath);
	spdlog::info("Folder for data found");

	// create folder for game if it does not exist
	fs::path folderPath = targetPath / _processName;
	fs::create_directories(folderPath);
	spdlog::info("Folder for game found");

	// build file path to save to
	filePathTimes = folderPath / uniqueFileName(folderPath, "times");
	filePathRates = folderPath / uniqueFileName(folderPath, "rates");
	filePathBaseline = folderPath / uniqueFileName(folderPath, "durations");
}

std::vector<long long> getFrameTimeVector(frametimeArray _times) {
	std::vector<long long> times;
	for (const auto& time : _times) {
		times.push_back(time.count());
	}
	return times;
}

void ResultsHandler::addBaseline(frametimeArray _baselineTimes, durationArray _baselineDurations, std::array<int, D3D11_METHOD_COUNT> _calls){
	json f_vector(getFrameTimeVector(_baselineTimes));
	outputJsonTimes["baseline"] = f_vector;

	for (int i = 0; i < D3D11_METHOD_COUNT; i++) {
		outputJsonBaseline["durations"][methodNames[i]] = _baselineDurations[i].count();
		outputJsonBaseline["calls"][methodNames[i]] = _calls[i];
	}

	spdlog::info("Baseline times added to file");
}

void ResultsHandler::addResultSingle(frametimeArray _frameTimes, frametimeArray _frameRates, int _methodIndex, float _speedup){
	string locM = methodNames[_methodIndex];
	string locS = to_string(static_cast<int>(_speedup * 100));

	json t_vector(getFrameTimeVector(_frameTimes));
	outputJsonTimes[locM][locS] = t_vector;

	json r_vector(getFrameTimeVector(_frameRates));
	outputJsonRates[locM][locS] = r_vector;
	spdlog::info("Results for {} with {}% delay added to file", locM, static_cast<int>(_speedup * 100));
}

void ResultsHandler::addResultAll(frametimeArray _frameTimes, frametimeArray _frameRates, float _speedup) {
	string locM = "all";
	string locS = to_string(static_cast<int>(_speedup * 100));

	json t_vector(getFrameTimeVector(_frameTimes));
	outputJsonTimes[locM][locS] = t_vector;

	json r_vector(getFrameTimeVector(_frameRates));
	outputJsonRates[locM][locS] = r_vector;

	spdlog::info("Results for all methods with {}% delay added to file", static_cast<int>(_speedup * 100));
}

void ResultsHandler::addThreadIDs(std::map<int, std::vector<idHash>> _ids){
	json id_vector;
	for (const auto& elem : _ids) {
		spdlog::info("Method {} called by {} different threads", methodNames[elem.first], elem.second.size());
		for (const auto& thread : elem.second) {
			outputJsonBaseline["threads"][methodNames[elem.first]].push_back(thread);
		}
	}
}

void ResultsHandler::exportResults(){
	spdlog::info("Writing Data to {} and {}", filePathTimes.string(), filePathRates.string());

	std::ofstream outputFile;
	outputFile.open(filePathTimes.string(), std::ios::out | std::ios::trunc);
	outputFile << outputJsonTimes.dump();
	outputFile.close();

	outputFile.open(filePathRates.string(), std::ios::out | std::ios::trunc);
	outputFile << outputJsonRates.dump();
	outputFile.close();

	outputFile.open(filePathBaseline.string(), std::ios::out | std::ios::trunc);
	outputFile << outputJsonBaseline.dump();
	outputFile.close();

	spdlog::info("Done writing data");
}
