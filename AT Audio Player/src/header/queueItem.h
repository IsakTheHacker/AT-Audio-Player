#pragma once

#include <string>

#include "song.h"
#include "playlist.h"

class QueueItem {
private:
	Song song;
	Playlist playlist;
	std::string name = "No name defined";
	int contents = -1;			//Describes what this queue item consists of: -1 means empty, 0 is a song and 1 means a playlist
public:
	QueueItem() { }
	QueueItem(Song song) {
		this->song = song;
		contents = 0;
		name = song.getName();
	}
	QueueItem(Playlist playlist) {
		this->playlist = playlist;
		contents = 1;
		name = playlist.getName();
	}
	int getContents() {
		return contents;
	}
	Song getSong() {
		return song;
	}
	Playlist getPlaylist() {
		return playlist;
	}
	std::string getName() { return name; }
};