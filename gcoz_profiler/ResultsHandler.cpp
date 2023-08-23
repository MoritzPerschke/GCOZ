#include "ResultsHandler.h"

string uniqueFileName(fs::path& _dir) {
	string base = "data";
	string ext = ".json";
	std::string unique = base + ext;
	int counter = 0;

	do {
		std::ostringstream oss;
		oss << base << "_" << counter << ext;
		unique = oss.str();
		counter++;
	}while (fs::exists(_dir / unique));

	return unique;
}

ResultsHandler::ResultsHandler() {
}

ResultsHandler::ResultsHandler(string& _processName){
	// create data dir if it does not exist
	fs::path targetPath = "../../../data";
	fs::create_directories(targetPath);
	std::cout << ok << "[ResultsHandler] Data folder found" << std::endl;

	// create folder for game if it does not exist
	fs::path folderPath = targetPath / _processName;
	fs::create_directories(folderPath);
	std::cout << ok << "[ResultsHandler] Game folder found" << std::endl;

	// build file path to save to
	filePath = folderPath / uniqueFileName(folderPath);
}

std::vector<long long> getTimeVector(frametimeArray _times) {
	std::vector<long long> times;
	for (const auto& time : _times) {
		times.push_back(time.count());
	}
	return times;
}

void ResultsHandler::addBaseline(frametimeArray _baselineTimes){
	json j_vector(getTimeVector(_baselineTimes));
	outputJson["baseline"] = j_vector;
	std::cout << ok << "[ResultsHandler] Baseline times added" << std::endl;
}

void ResultsHandler::addResultSingle(frametimeArray _frameTimes, int _methodIndex, float _speedup){
	string locM = "method" + to_string(_methodIndex);
	string locS = to_string(static_cast<int>(_speedup * 100));

	json j_vector(getTimeVector(_frameTimes));
	outputJson[locM][locS] = j_vector;
	std::cout << ok << "[ResultsHandler] Results for single method(" << _methodIndex << ") added with delay " << static_cast<int>(_speedup * 100) << "%" << std::endl;
}

void ResultsHandler::addResultAll(frametimeArray _frameTimes, float _speedup) {
	string locM = "all";
	string locS = to_string(static_cast<int>(_speedup * 100));
	json j_vector(getTimeVector(_frameTimes));
	outputJson[locM][locS] = j_vector;
	std::cout << ok << "[ResultsHandler] Results for all methods added with delay " << static_cast<int>(_speedup * 100) << "%" << std::endl;
}

void ResultsHandler::exportResults(){
	std::cout << inf << "[ResultsHandler] Writing Data to: " << filePath << std::endl;
	std::ofstream outputFile;
	outputFile.open(filePath.string(), std::ios::out | std::ios::trunc);
	outputFile << outputJson.dump();
	outputFile.close();
	std::cout << ok << "[ResultsHandler] Done" << std::endl;
}
