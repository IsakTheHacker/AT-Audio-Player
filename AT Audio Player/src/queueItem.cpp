#include "queueItem.h"

QueueItem::QueueItem() {
	
}
QueueItem::QueueItem(Song song) {
	this->song = song;
	contents = 0;
	name = song.getName();
}
QueueItem::QueueItem(Playlist playlist) {
	this->playlist = playlist;
	contents = 1;
	name = playlist.getName();
}
int QueueItem::getContents() {
	return contents;
}
Song QueueItem::getSong() {
	return song;
}
Playlist QueueItem::getPlaylist() {
	return playlist;
}
std::string QueueItem::getName() { return name; }