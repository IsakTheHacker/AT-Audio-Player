#include "playbackController.h"

//Private
void PlaybackController::playLoadedItem() {
	if (loadedItem.getContents() == 0) {			//Song
		sound = engine->play2D(loadedItem.getSong().getSource(), false, false, true);
	}
	else if (loadedItem.getContents() == 1) {		//Playlist
		sound = engine->play2D(loadedItem.getPlaylist().popFront().getSource(), false, false, true);
	}
	sound->setVolume((irrklang::ik_f32)0.7);
}
void PlaybackController::pause() { engine->setAllSoundsPaused(true); }
void PlaybackController::unpause() { engine->setAllSoundsPaused(false); }

//Public
void PlaybackController::set(QueueItem queueItem) {
	loadedItem = queueItem;
}
void PlaybackController::set(Song song) {
	loadedItem = QueueItem(song);
}
void PlaybackController::set(Playlist playlist) {
	loadedItem = QueueItem(playlist);
}
void PlaybackController::setQueue(Queue& queue) { this->queue = &queue; }
Queue PlaybackController::getQueue() { return *queue; }
void PlaybackController::switchPauseMode() {
	if (!sound) return;
	if (sound->getIsPaused() == true) {		//Sound is paused, unpause
		unpause();
	}
	else {
		pause();
	}
}
bool PlaybackController::isPlaying() {
	if (!sound) return false;
	if (engine->isCurrentlyPlaying(sound->getSoundSource())) {
		return true;
	}
	else {
		return false;
	}
}
void PlaybackController::skip() {
	if (!sound) return;
	sound->stop();
}
void PlaybackController::rewind() {
	if (!sound) return;
	sound->setPlayPosition(0);
}
void PlaybackController::switchRepeatMode() {
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
void PlaybackController::setVolume(float volume) {
	if (!sound) return;
	if (volume > 10 || volume < 0) return;
	sound->setVolume(volume / 10);
}
float PlaybackController::getVolume() {
	if (!sound) return 5;
	return sound->getVolume() * 10;
}
Song PlaybackController::getSong() {
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
void PlaybackController::pullFromQueue(PlaybackController* playbackController, Queue* queue) {
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