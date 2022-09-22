#include "playlist.h"
#include "handyFunctions.h"
#include <filesystem>
namespace fs = std::filesystem;

void Playlist::loadSongs(std::vector<Song> songs) {
	for (Song song : songs) {
		this->songs.push(song);
	}
}
void Playlist::loadSongs(std::vector<std::string> songPaths) {
	for (std::string path : songPaths) {
		this->songs.push(Song(path));
	}
}
void Playlist::loadSongs(std::string path) {
	int inputType = getInputType(path);
	if (inputType == 1) {																//Folder playlist
		for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
			this->songs.push(Song(entry.path().string()));
		}
	}
	else if (inputType == 2) {															//JSON playlist
		
	}
}
Song Playlist::getFront() {
	return songs.front();
}
Song Playlist::popFront() {
	Song song = songs.front();
	songs.pop();
	return song;
}
bool Playlist::getEmpty() {
	if (!songs.size()) return true;
	return false;
}
std::string Playlist::getName() { return name; }