#pragma once

class Console {
public:
	static void clear();
	static void setCursorPos(int x = 0, int y = 0);
	static void showCursor(const bool& show);
};