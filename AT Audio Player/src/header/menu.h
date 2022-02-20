#pragma once
#include <string>
#include <vector>

namespace curses {
	#include <ncurses.h>
}

#include "stdconfig.h"

class Menu1d {
private:
	int yMax, xMax;
	int height, width, starty, startx;
	curses::WINDOW* window;
	std::vector<std::string> choices;

	unsigned short getHighestLength(const std::vector<std::string>& vec);
public:
	Menu1d(const std::vector<std::string>& choices);
	int waitForSelection(const int& startAt = 0, const bool& allowEscExit = true);
};

// class Menu2d {
// private:
// 	int yMax, xMax;
// 	int height, width, starty, startx;
// 	curses::WINDOW* window;
// 	std::vector<std::string> choices;

// 	unsigned short getHighestLength(const std::vector<std::string>& vec);
// public:
// 	Menu2d(const std::vector<std::string>& choices);
// 	int waitForSelection(const int& startAtY = 0, const int& startAtX = 0, const bool& allowEscExit = true);
// };