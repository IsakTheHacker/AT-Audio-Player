#pragma once

#include <iostream>

#include <ik_ISound.h>
#include <ik_ISoundEngine.h>

#include "queue.h"
#include "queueItem.h"
#include "song.h"
#include "playlist.h"
#include "globalVars.h"

class PlaybackController {
private:
	irrklang::ISound* sound = nullptr;

	Queue* queue = nullptr;
	QueueItem loadedItem;

	void playLoadedItem() {
		if (loadedItem.getContents() == 0) {			//Song
			sound = engine->play2D(loadedItem.getSong().getSource(), false, false, true);
		}
		else if (loadedItem.getContents() == 1) {		//Playlist
			sound = engine->play2D(loadedItem.getPlaylist().popFront().getSource(), false, false, true);
		}
		sound->setVolume(0.7);
	}
	void pause() { engine->setAllSoundsPaused(true); }
	void unpause() { engine->setAllSoundsPaused(false); }
public:
	void set(QueueItem queueItem) {
		loadedItem = queueItem;
	}
	void set(Song song) {
		loadedItem = QueueItem(song);
	}
	void set(Playlist playlist) {
		loadedItem = QueueItem(playlist);
	}
	void setQueue(Queue& queue) { this->queue = &queue; }
	Queue getQueue() { return *queue; }
	void switchPauseMode() {
		if (!sound) return;
		if (sound->getIsPaused() == true) {		//Sound is paused, unpause
			unpause();
		}
		else {
			pause();
		}
	}
	bool isPlaying() {
		if (!sound) return false;
		if (engine->isCurrentlyPlaying(sound->getSoundSource())) {
			return true;
		}
		else {
			return false;
		}
	}
	void skip() {
		if (!sound) return;
		sound->stop();
	}
	void rewind() {
		if (!sound) return;
		sound->setPlayPosition(0);
	}
	void switchRepeatMode() {
		if (!sound) return;
		if (sound->isLooped() == true) {
			sound->setIsLooped(false);
			std::cout << "The song is no longer on repeat." << std::endl;
		}
		else {
			sound->setIsLooped(true);
			std::cout << "The song has been set on repeat!" << std::endl;
		}
	}
	void setVolume(float volume) {
		if (!sound) return;
		if (volume > 10 || volume < 0) return;
		sound->setVolume(volume / 10);
	}
	float getVolume() {
		if (!sound) return 5;
		return sound->getVolume() * 10;
	}
	Song getSong() {
		if (!sound) {
			return Song();
		}
		if (loadedItem.getContents() == 0) {			//Song
			return loadedItem.getSong();
		}
		else if (loadedItem.getContents() == 1) {		//Playlist
			return loadedItem.getPlaylist().getFront();
		}
	}
	static void pullFromQueue(PlaybackController* playbackController, Queue* queue) {
		while (shouldRun) {
			if (playbackController->isPlaying()) continue;						//Check if currently playing
			if (playbackController->loadedItem.getContents() == 1) {			//Check if playlist is empty
				if (!playbackController->loadedItem.getPlaylist().getEmpty()) {
					playbackController->playLoadedItem();
					continue;
				}
			}
			if (queue->getEmpty()) continue;									//Check if queue is empty

			playbackController->set(queue->popFront());
			playbackController->playLoadedItem();
		}
	}
};