#include "ResultsHandler.h"

std::wstring getProcessName(DWORD _pid) {
	std::wstring result = L"";
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, _pid);
	TCHAR buffer[MAX_PATH];
	if (hProcess){
		std::cout << "Got Handle to process" << std::endl;
		if(GetModuleFileNameEx(hProcess, 0, buffer, MAX_PATH)) {
			std::cout << "Got Process Name" << std::endl;
			for (char c : buffer) {
				result += c;
				std::cout << c;
			}
			std::cout << std::endl;
		}
		CloseHandle(hProcess);
	}
	return result;
}

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
	std::cout << ok << "Data folder found" << std::endl;

	// create folder for game if it does not exist
	fs::path folderPath = targetPath / _processName;
	fs::create_directories(folderPath);
	std::cout << ok << "Game folder found" << std::endl;

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
	std::cout << ok << "Baseline times added" << std::endl;
}

void ResultsHandler::addResultSingle(frametimeArray _frameTimes, int _methodIndex, float _speedup){
	string locM = "method" + to_string(_methodIndex);
	string locS = to_string(_speedup);

	json j_vector(getTimeVector(_frameTimes));
	outputJson[locM][locS] = j_vector;
	std::cout << ok << "Results for single method(" << _methodIndex << ") added with delay " << _speedup << std::endl;
}

void ResultsHandler::addResultAll(frametimeArray _frameTimes, float _speedup){
	string locM = "all";
	string locS = to_string(_speedup);
	json j_vector(getTimeVector(_frameTimes));
	outputJson[locM][locS] = j_vector;
	std::cout << ok << "Results for all methods added with delay " << _speedup << std::endl;
}

void ResultsHandler::exportResults(){
	std::ofstream outputFile(filePath.string(), std::ios::out | std::ios::trunc);
	std::cout << "Resulting Data: " << std::endl;
	std::cout << outputJson.dump();
	outputFile << outputJson.dump();
}
