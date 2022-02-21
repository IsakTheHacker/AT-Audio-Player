#include "menu.h"

SelectionMenu1d::SelectionMenu1d(const std::vector<std::string>& choices) {
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

int SelectionMenu1d::waitForSelection(const int& startAt, const bool& allowEscExit) {
	using namespace curses;
	wrefresh(window);
	keypad(window, true);

	int choice;
	int highlight = startAt;
	int returnFlag = -1;

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

		if (choice == 10) {								//Select option on Enter
			returnFlag = 0;
			break;
		} else if (choice == 27 && allowEscExit) {		//Close without selection on Escape
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

InputMenu::InputMenu(const int& height, const int& width) {
	using namespace curses;
	getmaxyx(stdscr, yMax, xMax);		//Get screen size

	//Calculate window size based on number of choices and text length
	this->height = height;
	this->width = width;

	starty = (yMax - height) / 2; 		//Calculating for a center placement
	startx = (xMax - width) / 2;  		//of the window
	window = newwin(height, width, starty, startx);
	wborder(window, '|', '|', '-', '-', '+', '+', '+', '+');
	this->stdText = stdText;
}

std::string InputMenu::waitForInput(const int& startAtY, const int& startAtX, const std::string& stdText, const bool& allowEscExit) {
	using namespace curses;
	curs_set(1);						//Show cursor
	echo();
	mvwprintw(window, startAtY, startAtX, stdText.c_str());
	wmove(window, startAtY, startAtX);
	wrefresh(window);

	int c;
	std::string input;
	int returnFlag = -1;

	while (true) {
		c = wgetch(window);

		if (c == 10) {								//Return with input on Enter
			returnFlag = 0;
			break;
		} else if (c == 27 && allowEscExit) {		//Close without input on Escape
			returnFlag = -1;
			break;
		} else if (c == KEY_BACKSPACE) {
			returnFlag = -1;
			break;
		} else {
			input += c;
		}
	}
	curs_set(0);						//Hide cursor again
	noecho();
	if (returnFlag == 0) {
		return input;
	} else {
		return "";
	}
}
