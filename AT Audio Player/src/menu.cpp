#include "menu.h"

SelectionMenu1d::SelectionMenu1d(const std::vector<std::string>& choices) {
	using namespace curses;
	getmaxyx(stdscr, yMax, xMax);		//Get screen size

	//Calculate window size based on number of choices and text length
	height = choices.size() + 2;
	width = this->getHighestLength(choices) + 2;

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
	noecho();
	keypad(window, true);
	nodelay(window, false);				//Wait for characters to be pressed (blocking)
	mvwprintw(window, startAtY, startAtX, stdText.c_str());
	wmove(window, startAtY, startAtX);
	wrefresh(window);

	int c;
	std::string input;
	int returnFlag = -1;
	int cursorY, cursorX;

	while (true) {
		c = wgetch(window);

		if (c == 10 || c == '\n') {										//Return with input on Enter
			returnFlag = 0;
			break;
		} else if (c == 27 && allowEscExit) {							//Close without input on Escape
			returnFlag = -1;
			break;
		} else if (c == KEY_BACKSPACE || c == KEY_DC || c == 127) {		//Adds support for deleting typed chars
			getyx(window, cursorY, cursorX);
    		int cursorPos = cursorX-startAtX;
			if (!input.empty() && cursorPos > 0) {
				input.erase(cursorPos-1, 1);
				cursorX -= 1;
				wmove(window, cursorY, cursorX);
			}
		} else if (c == KEY_LEFT) {
			getyx(window, cursorY, cursorX);
			if (cursorX > startAtX) {
				cursorX -= 1;
				wmove(window, cursorY, cursorX);
			}
		} else if (c == KEY_RIGHT) {
			getyx(window, cursorY, cursorX);
			if (cursorX < input.size()+startAtX) {
				cursorX += 1;
				wmove(window, cursorY, cursorX);
			}
		} else {
			getyx(window, cursorY, cursorX);
			int cursorPos = cursorX-startAtX;
			std::string preStr = input.substr(0, cursorPos);
			std::string postStr = input.substr(cursorPos, input.length() - preStr.length());
			input = preStr + (char)c + postStr;
			cursorX += 1;
			wmove(window, cursorY, cursorX);
		}

		//Redraw typed chars
		if (input.length() > 0) {
			getyx(window, cursorY, cursorX);
			mvwprintw(window, startAtY, startAtX, (input + concatString(" ", width - startAtX*2 - input.size())).c_str());
			wmove(window, cursorY, cursorX);
		} else {
			mvwprintw(window, startAtY, startAtX, stdText.c_str());
			wmove(window, startAtY, startAtX);
		}
		wrefresh(window);
	}
	curs_set(0);						//Hide cursor again
	if (returnFlag == 0) {
		return input;
	} else {
		return "";
	}
}
