#pragma once
#include <string>
#include <vector>
#include <conterlib.hpp>

#include "stdconfig.h"
#include "handyFunctions.h"

class SelectionMenu1d {
private:
	int height, width, starty, startx;
	TUI::Window window;
	std::vector<std::string> choices;

	unsigned short getHighestLength(const std::vector<std::string>& vec);
public:
	SelectionMenu1d(const std::vector<std::string>& choices);
	int waitForSelection(const int& startAt = 0, const bool& allowEscExit = true);
};

// class SelectionMenu2d {
// private:
// 	int yMax, xMax;
// 	int height, width, starty, startx;
// 	curses::WINDOW* window;
// 	std::vector<std::string> choices;

// 	unsigned short getHighestLength(const std::vector<std::string>& vec);
// public:
// 	SelectionMenu2d(const std::vector<std::string>& choices);
// 	int waitForSelection(const int& startAtY = 0, const int& startAtX = 0, const bool& allowEscExit = true);
// };