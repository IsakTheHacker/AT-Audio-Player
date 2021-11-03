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
	void loadSongs(std::vector<Song> songs) {
		for (Song song : songs) {
			this->songs.push(song);
		}
	}
	void loadSongs(std::vector<std::string> songPaths) {
		for (std::string path : songPaths) {
			this->songs.push(Song(path));
		}
	}
	void loadSongs(std::string path) {
		int inputType = getInputType(path);
		if (inputType == 1) {
			//Folder playlist
		} else if (inputType == 2) {
			//JSON playlist
		}
	}
	Song getFront() {
		return songs.front();
	}
	Song popFront() {
		Song song = songs.front();
		songs.pop();
		return song;
	}
	bool getEmpty() {
		if (!songs.size()) return true;
		return false;
	}
	std::string getName() { return name; }
};