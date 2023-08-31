// TimerStats.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "TimerStats.h"

long long little_sleep_us(DWORD delay) { // https://stackoverflow.com/a/45571538
	auto start = timer.now();
	auto end = start + static_cast<Microseconds>(delay);
	auto loopStart = timer.now();
	while (timer.now() < end) {
		if ((timer.now() - end) > std::chrono::milliseconds(1)) {
			std::this_thread::yield();
		}
	}
	Microseconds loopEnd = std::chrono::duration_cast<Microseconds>(timer.now() - loopStart);
	return loopEnd.count();
}

long long little_sleep_ns(DWORD delay) {
	auto start = timer.now();
	auto end = start + static_cast<Nanoseconds>(delay);
	auto loopStart = timer.now();
	while (timer.now() < end) {
		if ((timer.now() - end) > std::chrono::milliseconds(1)) {
			std::this_thread::yield();
		}
	}
	Nanoseconds loopEnd = std::chrono::duration_cast<Nanoseconds>(timer.now() - loopStart);
	return loopEnd.count();
}

float run(DWORD delay, int runs, sleep_function func, json& outputJson) {
	std::vector<long long> results;

	for (int i = 0; i < runs; i++) {
		results.push_back(func(delay));
	}

	std::string unit = (func == &little_sleep_ns ? "ns" : "us");
	std::string times = std::to_string(delay);
	outputJson[unit][times] = results;

	float result = 0.0;
	for (const auto& time : results) {
		result += static_cast<float>(time) / results.size();
	}
	return result;
}

int main(int argc, char* argv[])
{
	std::cout << ok << "Creating Json of time runs..." << std::endl;
	// create data dir if it does not exist
	fs::path targetPath = "../../../data";
	fs::create_directories(targetPath);

	// create folder for game if it does not exist
	fs::path folderPath = targetPath / "timer";
	fs::create_directories(folderPath);

	// build file path to save to
	fs::path filePath = folderPath / "timerResults.json";

	json outputJson;

	LARGE_INTEGER ilFreq;
	QueryPerformanceFrequency(&ilFreq);
	std::cout << ok << "QueryPerformanceFrequency result: " << ilFreq.QuadPart << std::endl;
	std::cout << inf << "\"Clock\" resolution should be: " << 1. / ilFreq.QuadPart << " seconds, "
		<< (1. / ilFreq.QuadPart) * (1 * 10e6) << " Microseconds, "
		<< (1. / ilFreq.QuadPart) * (1 * 10e9) << " Nanoseconds" << std::endl;

	int runs;
	std::cout << ok << "Running Benchmark..." << std::endl;

	runs = 1e0;
	std::cout << inf << runs << " Runs:(1): \n"
		<< inf << run(1, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(1, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(10): \n"
		<< inf << run(10, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(10, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(100): \n"
		<< inf << run(100, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(100, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(1000): \n"
		<< inf << run(1000, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(1000, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(10000): \n"
		<< inf << run(10000, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(10000, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;


	runs = 1e1;
	std::cout << inf << runs << " Runs:(1): \n"
		<< inf << run(1, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(1, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(10): \n"
		<< inf << run(10, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(10, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(100): \n"
		<< inf << run(100, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(100, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(1000): \n"
		<< inf << run(1000, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(1000, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(10000): \n"
		<< inf << run(10000, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(10000, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	/* ====================  Noone said this was _good_ code  ====================*/
	runs = 1e2;
	std::cout << inf << runs << " Runs:(1): \n"
		<< inf << run(1, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(1, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(10): \n"
		<< inf << run(10, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(10, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(100): \n"
		<< inf << run(100, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(100, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(1000): \n"
		<< inf << run(1000, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(1000, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(10000): \n"
		<< inf << run(10000, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(10000, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;

	runs = 1e3;
	std::cout << inf << runs << " Runs:(1): \n"
		<< inf << run(1, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(1, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(10): \n"
		<< inf << run(10, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(10, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(100): \n"
		<< inf << run(100, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(100, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(1000): \n"
		<< inf << run(1000, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(1000, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;
	std::cout << inf << runs << " Runs:(10000): \n"
		<< inf << run(10000, runs, &little_sleep_ns, outputJson) << "ns" << std::endl
		<< inf << run(10000, runs, &little_sleep_us, outputJson) << "us\n" << std::endl;

	if (argc == 2) {
		std::cout << ok << "Measurements done, writing file out..." << std::endl;
		std::ofstream outputFile;
		outputFile.open(filePath.string(), std::ios::out | std::ios::trunc);
		outputFile << outputJson.dump();
		outputFile.close();
		std::cout << ok << "Writing done, have fun with python" << std::endl;
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
