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
	QueueItem();
	QueueItem(Song song);
	QueueItem(Playlist playlist);
	int getContents();
	Song getSong();
	Playlist getPlaylist();
	std::string getName();
};