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

InputMenu::InputMenu(const int& height, const int& width) {
	TUI::WindowSize screenSize = Console::getScreenSize();

	//Calculate window size based on number of choices and text length
	this->height = height;
	this->width = width;

	starty = screenSize.ySize / 2 - height / 2;		//Calculating for a center placement
	startx = screenSize.xSize / 2 - width / 2;		//of the window
	window = TUI::Window(width, height, startx, starty);
	this->stdText = stdText;
}

std::string InputMenu::waitForInput(const int& startAtY, const int& startAtX, const std::string& stdText, const bool& allowEscExit) {
	Console::showCursor(true);
	Console::setCursorPos(window.getStartPos().x + startAtX, window.getStartPos().y + startAtY);
	std::cout << stdText;
	Console::setCursorPos(window.getStartPos().x + startAtX, window.getStartPos().y + startAtY);

	Key c;
	std::string input;
	int returnFlag = -1;
	TUI::Coord cursor;

	while (true) {
		c = Console::readKey();

		if (c == Key::ENTER) {										//Return with input on Enter
			returnFlag = 0;
			break;
		} else if (c == Key::ESC && allowEscExit) {							//Close without input on Escape
			returnFlag = -1;
			break;
		} else if (c == Key::BACKSPACE || c == Key::DEL) {		//Adds support for deleting typed chars
			cursor = Console::getCursorPos();
			int cursorPos = cursor.x - window.getStartPos().x - startAtX;
			if (!input.empty() && cursorPos > 0) {
				input.erase(cursorPos-1, 1);
				cursor.x -= 1;
				Console::setCursorPos(cursor);
			}
		} else if (c == Key::ARROW_LEFT) {
			cursor = Console::getCursorPos();
			if (cursor.x > startAtX) {
				cursor.x -= 1;
				Console::setCursorPos(cursor);
			}
		} else if (c == Key::ARROW_RIGHT) {
			cursor = Console::getCursorPos();
			if (cursor.x < input.size()+startAtX) {
				cursor.x += 1;
				Console::setCursorPos(cursor);
			}
		} else {
			cursor = Console::getCursorPos();
			int cursorPos = cursor.x - window.getStartPos().x - startAtX;
			std::string preStr = input.substr(0, cursorPos);
			std::string postStr = input.substr(cursorPos, input.length() - preStr.length());
			input = preStr + (char)c + postStr;
			cursor.x += 1;
			Console::setCursorPos(cursor);
		}

		//Redraw typed chars
		if (input.length() > 0) {
			cursor = Console::getCursorPos();
			Console::setCursorPos(window.getStartPos().x + startAtX, window.getStartPos().y + startAtY);
			std::cout << input + concatString(" ", width - startAtX * 2 - input.size());
			Console::setCursorPos(cursor);
		} else {
			Console::setCursorPos(window.getStartPos().x + startAtX, window.getStartPos().y + startAtY);
			std::cout << stdText;
			Console::setCursorPos(window.getStartPos().x + startAtX, window.getStartPos().y + startAtY);
		}
	}
	Console::showCursor(false);
	if (returnFlag == 0) {
		return input;
	} else {
		return "";
	}
}
