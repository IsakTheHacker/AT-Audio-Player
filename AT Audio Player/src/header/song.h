#pragma once

#include <string>

class Song {
private:
	std::string path = "Not set";
	std::string name = "None";
public:
	Song();
	Song(std::string path);
	std::string getPath();
	std::string getName();
};