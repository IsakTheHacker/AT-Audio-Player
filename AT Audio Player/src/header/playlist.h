#pragma once

#include <string>
#include <queue>

#include "song.h"
#include "handyFunctions.h"

class Playlist {
private:
	std::queue<Song> songs;
	std::string name = "Playlist";
public:
	void loadSongs(std::vector<Song> songs);
	void loadSongs(std::vector<std::string> songPaths);
	void loadSongs(std::string path);
	Song getFront();
	Song popFront();
	bool getEmpty();
	std::string getName();
};