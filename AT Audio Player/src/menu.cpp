#include "menu.h"

Menu1d::Menu1d(const std::vector<std::string>& choices) {
	using namespace curses;
	getmaxyx(stdscr, yMax, xMax);		//Get screen size

	//Calculate window size based on number of choices and text length
	height = choices.size() + 2;
	width = this->getHighestLength(choices);

	starty = (yMax - height) / 2; 		//Calculating for a center placement
	startx = (xMax - width) / 2;  		//of the window
	window = newwin(height, width, starty, startx);
	wborder(window, '|', '|', '-', '-', '+', '+', '+', '+');
	this->choices = choices;
}

int Menu1d::waitForSelection(const int& startAt, const bool& allowEscExit) {
	using namespace curses;
	wrefresh(window);
	keypad(window, true);

	int choice;
	int highlight = startAt;

	while (true) {
		for (size_t i = 0; i < choices.size(); i++) {
			if (i == highlight)
				wattron(window, A_REVERSE);
			mvwprintw(window, i+1, 1, choices[i].c_str());
			wattroff(window, A_REVERSE);
		}
		choice = wgetch(window);

		switch (choice) {
		case KEY_UP:
			if (highlight > 0) {
				highlight--;
			}
			break;
		case KEY_DOWN:
			if (highlight < choices.size()-1) {
				highlight++;
			}
			break;
		default:
			break;
		}

		if (choice == 10) {						//Select option on Enter
			return highlight;
		}
		if (choice == 27 && allowEscExit) {		//Close without selection on Escape
			return -1;
		}
	}
}

unsigned short Menu1d::getHighestLength(const std::vector<std::string>& vec) {
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