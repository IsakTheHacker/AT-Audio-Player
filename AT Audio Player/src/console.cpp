#include "console.h"

//Windows
#if defined(_WIN32) || defined(_WIN64)
#include "Windows.h"

void Console::clear() {
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	//Get the number of cells in the current buffer
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	//Fill the entire buffer with spaces
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	//Fill the entire buffer with the current colors and attributes
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	SetConsoleCursorPosition(hStdOut, homeCoords);							//Move the cursor home
}

void Console::setCursorPos(int x, int y) {
	HANDLE handle;
	COORD coordinates;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	coordinates.X = x;
	coordinates.Y = y;
	SetConsoleCursorPosition(handle, coordinates);
}

void Console::showCursor(const bool& show) {
	static const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(handle, &cci);
	cci.bVisible = show;													//Show/hide cursor
	SetConsoleCursorInfo(handle, &cci);
}

void Console::showScrollbar(const bool& show) {
	ShowScrollBar(GetConsoleWindow(), SB_VERT, show);
}

//Linux
#elif defined(__linux__)
#include <iostream>

void Console::showCursor(const bool& show) {
	std::cout << (show ? "\033[?25h" : "\033[?25l");						//Show/hide cursor
}

#endif