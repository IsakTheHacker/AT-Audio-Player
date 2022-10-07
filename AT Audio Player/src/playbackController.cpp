#include "playbackController.h"
#include "application.h"
#include <soloud_wav.h>

//Private
void PlaybackController::playLoadedItem() {
	if (loadedItem.getContents() == 0) {			//Song
		song = new sl::Wav;
		song->load(loadedItem.getSong().getPath().c_str());
		sound = Application::engine->play(*song);
	} else if (loadedItem.getContents() == 1) {		//Playlist
		song = new sl::Wav;
		song->load(loadedItem.getPlaylist().popFront().getPath().c_str());
		sound = Application::engine->playBackground(*song);
	}
}
void PlaybackController::pause() { Application::engine->setPauseAll(true); }
void PlaybackController::unpause() { Application::engine->setPauseAll(false); }

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
	if (Application::engine->getPause(sound) == true) {		//Sound is paused, unpause
		unpause();
	} else {
		pause();
	}
}
bool PlaybackController::isPlaying() {
	if (!sound) return false;
	if (Application::engine->getActiveVoiceCount() != 0) {
		return true;
	}
	else {
		return false;
	}
}
void PlaybackController::skip() {
	if (!sound) return;
	Application::engine->stop(sound);
}
void PlaybackController::rewind() {
	if (!sound) return;
	Application::engine->seek(sound, 0);
}
void PlaybackController::seek(double seconds) {
	if (!sound) return;
	if (seconds < 0)
		Application::engine->seek(sound, 0);
	else
		Application::engine->seek(sound, seconds);
}
double PlaybackController::getPlaytime() {
	if (!sound) return 0;
	return Application::engine->getStreamPosition(sound);
}
void PlaybackController::switchRepeatMode() {
	if (!sound) return;
	/*if (sound->isLooped() == true) {
		sound->setIsLooped(false);
		std::cout << "The song is no longer on repeat." << std::endl;
	}
	else {
		sound->setIsLooped(true);
		std::cout << "The song has been set on repeat!" << std::endl;
	}*/
}
void PlaybackController::setVolume(float volume) {
	if (volume > 10 || volume < 0) return;
	Application::engine->setGlobalVolume(volume / 10);
}
float PlaybackController::getVolume() {
	return Application::engine->getGlobalVolume() * 10;
}
Song PlaybackController::getSong() {
	if (!sound) {
		return Song();
	}
	if (loadedItem.getContents() == 0) {			//Song
		return loadedItem.getSong();
	} else if (loadedItem.getContents() == 1) {		//Playlist
		return loadedItem.getPlaylist().getFront();
	}
	return Song();
}
void PlaybackController::pullFromQueue(PlaybackController* playbackController, Queue* queue) {
	while (Application::shouldRun) {
		if (playbackController->isPlaying()) continue;						//Check if currently playing
		if (playbackController->loadedItem.getContents() == 1 &&			//Check if playlist is empty
			!playbackController->loadedItem.getPlaylist().getEmpty()
		) {
			if (playbackController->song != nullptr) delete playbackController->song;
			playbackController->playLoadedItem();
			continue;
		}
		if (queue->getEmpty()) continue;									//Check if queue is empty

		if (playbackController->song != nullptr) delete playbackController->song;
		playbackController->set(queue->popFront());
		playbackController->playLoadedItem();
	}
}