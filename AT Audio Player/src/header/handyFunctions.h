#pragma once

#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

#include <irrKlang.h>

#include "globalVars.h"

//Returns 0 if song, 1 if folder playlist, 2 if JSON playlist and -1 if invalid path
int getInputType(const std::string& pathStr);

void initEngine();

std::string concatString(std::string subStr, int times);
std::string concatString(char subStr, int times);
