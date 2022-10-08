#pragma once

#include <iostream>
#include <soloud.h>
#include <soloud_wav.h>
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
	sl::Wav* song = nullptr;
	void set(QueueItem queueItem);
	void set(Song song);
	void set(Playlist playlist);
	void setQueue(Queue& queue);
	Queue getQueue();
	void switchPauseMode();
	bool isPlaying();
	void skip();
	void rewind();
	void seek(double seconds);
	double getPlaytime();
	double getPlayedPercentage();
	double getSongLength();
	void switchRepeatMode();
	void setVolume(float volume);
	float getVolume();
	Song getSong();
	static void pullFromQueue(PlaybackController* playbackController, Queue* queue);
};