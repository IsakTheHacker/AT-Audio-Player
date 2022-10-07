#include "handyFunctions.h"
#include <filesystem>
namespace fs = std::filesystem;

int getInputType(const std::string& pathStr) {
	const fs::path path(pathStr);
	if (!fs::exists(path)) {
		return -1;
	} else if (fs::is_directory(path)) {
		return 1;
	} else if (fs::is_regular_file(path)) {
		if (path.extension() == ".json") {
			return 2;
		} else {
			return 0;
		}
	}
	return -1;
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
