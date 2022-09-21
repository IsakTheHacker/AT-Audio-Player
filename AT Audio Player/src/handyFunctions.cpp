#include "handyFunctions.h"

int getInputType(const std::string& pathStr) {
	const fs::path path(pathStr);
	if (!fs::exists(path)) {
		return -1;
	}
	else if (fs::is_directory(path)) {
		return 1;
	}
	else if (fs::is_regular_file(path)) {
		if (path.extension() == ".json") {
			return 2;
		}
		else {
			return 0;
		}
	}
}

void initEngine() {
	engine = irrklang::createIrrKlangDevice();		//Start the engine
	if (!engine) {
		std::cout << "Error 01: Could not start engine" << std::endl;
		exit(1);							//Stop program execution
	}
}

std::string concatString(std::string subStr, int times) {
	std::string str;
	for (size_t i = 0; i < times; i++) {
		str += subStr;
	}
	return str;
}
std::string concatString(char subStr, int times) {
	std::string str;
	for (size_t i = 0; i < times; i++) {
		str += subStr;
	}
	return str;
}
