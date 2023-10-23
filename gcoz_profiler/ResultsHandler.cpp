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
	std::cout << err << "Resultshandler Constructor called without processName" << std::endl;
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

	std::cout << ok << "[ResultsHandler] Baseline times added" << std::endl;
}

void ResultsHandler::addResultSingle(frametimeArray _frameTimes, frametimeArray _frameRates, int _methodIndex, float _speedup){
	string locM = methodNames[_methodIndex];
	string locS = to_string(static_cast<int>(_speedup * 100));

	json t_vector(getFrameTimeVector(_frameTimes));
	outputJsonTimes[locM][locS] = t_vector;

	json r_vector(getFrameTimeVector(_frameRates));
	outputJsonRates[locM][locS] = r_vector;
	std::cout << ok << "[ResultsHandler] Results for single method(" << locM << ") added with delay " << static_cast<int>(_speedup * 100) << "%" << std::endl;
}

void ResultsHandler::addResultAll(frametimeArray _frameTimes, frametimeArray _frameRates, float _speedup) {
	string locM = "all";
	string locS = to_string(static_cast<int>(_speedup * 100));

	json t_vector(getFrameTimeVector(_frameTimes));
	outputJsonTimes[locM][locS] = t_vector;

	json r_vector(getFrameTimeVector(_frameRates));
	outputJsonRates[locM][locS] = r_vector;

	std::cout << ok << "[ResultsHandler] Results for all methods added with delay " << static_cast<int>(_speedup * 100) << "%" << std::endl;
}

void ResultsHandler::addThreadIDs(std::map<int, std::vector<idHash>> _ids){
	json id_vector;
	for (const auto& elem : _ids) {
		std::cout << inf << "Method " << methodNames[elem.first] << " called by " << elem.second.size() << " different threads" << std::endl;
		for (const auto& thread : elem.second) {
			outputJsonBaseline["threads"][methodNames[elem.first]].push_back(thread);
		}
	}
}

void ResultsHandler::exportResults(){
	std::cout << inf << "[ResultsHandler] Writing Data to: " << filePathTimes << " and " << filePathRates << std::endl;

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

	std::cout << ok << "[ResultsHandler] Done" << std::endl;
}
