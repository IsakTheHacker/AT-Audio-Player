#pragma once

#include <iostream>
#include <soloud.h>
namespace sl = SoLoud;

#include "queue.h"
#include "queueItem.h"
#include "song.h"
#include "playlist.h"

class PlaybackController {
private:
	sl::handle sound = 0;

	Queue* queue = nullptr;
	QueueItem loadedItem;

	void playLoadedItem();
	void pause();
	void unpause();
public:
	void set(QueueItem queueItem);
	void set(Song song);
	void set(Playlist playlist);
	void setQueue(Queue& queue);
	Queue getQueue();
	void switchPauseMode();
	bool isPlaying();
	void skip();
	void rewind();
	void switchRepeatMode();
	void setVolume(float volume);
	float getVolume();
	Song getSong();
	static void pullFromQueue(PlaybackController* playbackController, Queue* queue);
};