#include "menu.h"
#include <conterlib.hpp>
#include <iostream>

SelectionMenu1d::SelectionMenu1d(const std::vector<std::string>& choices) {
	TUI::WindowSize screenSize = Console::getScreenSize();

	//Calculate window size based on number of choices and text length
	height = choices.size() + 2;
	width = this->getHighestLength(choices) + 2;

	starty = (screenSize.ySize - height) / 2; 		//Calculating for a center placement
	startx = (screenSize.xSize - width) / 2;  		//of the window
	window = TUI::Window(width, height, startx, starty);
	this->choices = choices;
}

int SelectionMenu1d::waitForSelection(const int& startAt, const bool& allowEscExit) {
	Key choice;
	int highlight = startAt;
	int returnFlag = -1;

	while (true) {
		for (size_t i = 0; i < choices.size(); i++) {
			if (i == highlight)
				int i = 0;
				//wattron(window, A_REVERSE);
			//mvwprintw(window, i+1, 1, choices[i].c_str());
			//wattroff(window, A_REVERSE);
		}
		choice = Console::readKey();

		switch (choice) {
		case Key::ARROW_UP:
			if (highlight > 0) {
				highlight--;
			}
			break;
		case Key::ARROW_DOWN:
			if (highlight < choices.size()-1) {
				highlight++;
			}
			break;
		default:
			break;
		}

		if (choice == Key::ENTER) {								//Select option on Enter
			returnFlag = 0;
			break;
		} else if (choice == Key::ESC && allowEscExit) {		//Close without selection on Escape
			returnFlag = -1;
			break;
		}
	}
	if (returnFlag == 0) {
		return highlight;
	} else {
		return -1;
	}
}

unsigned short SelectionMenu1d::getHighestLength(const std::vector<std::string>& vec) {
	unsigned short longestChoice = stdconf::MENUWIN_MIN_WIDTH;
	for (std::string choice : vec) {
		if (choice.length() > longestChoice) {
			longestChoice = choice.length();
		}
	}
	unsigned short eval;
	if (stdconf::MENUWIN_MAX_WIDTH != 0) {
		eval = stdconf::MENUWIN_MAX_WIDTH;
	} else {
		eval = longestChoice;
	}
	if (longestChoice > eval) {
		longestChoice = eval;
	}
	return longestChoice;
}