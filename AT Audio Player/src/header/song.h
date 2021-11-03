#pragma once

#include <filesystem>
namespace fs = std::filesystem;

#include <irrKlang.h>

#include "globalVars.h"

class Song {
private:
	std::string path = "Not set";
	std::string name = "None";
	irrklang::ISoundSource* song = nullptr;
public:
	Song();
	Song(std::string path);
	std::string getPath();
	std::string getName();
	irrklang::ISoundSource* getSource();
};