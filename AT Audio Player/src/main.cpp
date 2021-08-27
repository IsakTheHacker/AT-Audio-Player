#pragma comment(lib, "irrKlang.lib") //Link with irrKlang.dll

//C++ std libs
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <thread>
#include <conio.h>
#include <filesystem>
namespace fs = std::filesystem;

//Thirdparty
#include <json.hpp>
#include <irrKlang.h>
using namespace irrklang;

//Global vars
ISoundEngine* engine;
bool shouldRun = true;

//Returns 0 if song, 1 if folder playlist, 2 if JSON playlist and -1 if invalid path
int getInputType(const std::string& pathStr) {
	const fs::path path(pathStr);
	if (!fs::exists(path)) {
		return -1;
	} else if (fs::is_directory(path)) {
		return 1;
	} else if (fs::is_regular_file(path)) {
		if (path.extension() == ".json") {
			return 2;
		} else {
			return 0;
		}
	}
}

void initEngine() {
	engine = createIrrKlangDevice();		//Start the engine
	if (!engine) {
		std::cout << "Error 01: Could not start engine" << std::endl;
		exit(1);							//Stop program execution
	}
}

class Song {
private:
	std::string path;
	ISoundSource* song;
public:
	Song() { }
	Song(std::string path) {
		song = engine->addSoundSourceFromFile(path.c_str(), ESM_AUTO_DETECT, true);		//Third parameter set to true == preload
	}
	std::string getPath() { return path; }
	std::string getName() { return song->getName(); }
	ISoundSource* getSource() { return song; }
};

class Playlist {
private:
	std::queue<Song> songs;
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
};

class QueueItem {
private:
	Song song;
	Playlist playlist;
	int contents = -1;			//Describes what this queue item consists of: -1 means empty, 0 is a song and 1 means a playlist
public:
	QueueItem() { }
	QueueItem(Song song) {
		this->song = song;
		contents = 0;
	}
	QueueItem(Playlist playlist) {
		this->playlist = playlist;
		contents = 1;
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
};

class Queue {
private:
	std::queue<QueueItem> queue;
public:
	void pushItem(QueueItem item) {
		queue.push(item);
	}
	void pushItem(Song song) {
		queue.push(QueueItem(song));
	}
	void pushItem(Playlist playlist) {
		queue.push(QueueItem(playlist));
	}
	QueueItem getFront() {
		return queue.front();
	}
	QueueItem popFront() {
		QueueItem queueItem = queue.front();
		queue.pop();
		return queueItem;
	}
	bool getEmpty() {
		if (!queue.size()) return true;
		return false;
	}
};

class Options {
private:
	bool useQueue;
	bool autoBackupOptionsAndData;
};

class PlaybackController {
private:
	ISound* sound;
	Queue* queue;
	QueueItem loadedItem;
	void playLoadedItem() {
		if (loadedItem.getContents() == 0) {			//Song
			sound = engine->play2D(loadedItem.getSong().getSource());
		} else if (loadedItem.getContents() == 1) {		//Playlist
			sound = engine->play2D(loadedItem.getPlaylist().popFront().getSource());
		}
	}
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
	void pause() { engine->setAllSoundsPaused(true); }
	void unpause() { engine->setAllSoundsPaused(false); }
	void setQueue(Queue& queue) { this->queue = &queue; }
	void switchPauseMode() {

	}
	bool isPlaying() {
		if (!sound) return false;
		if (engine->isCurrentlyPlaying(sound->getSoundSource())) {
			return true;
		} else {
			return false;
		}
	}
	static void pullFromQueue(PlaybackController* playbackController, Queue* queue) {
		while (shouldRun) {
			if (playbackController->isPlaying()) continue;					//Check if currently playing
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

int main(int argc, const char* argv[]) {
	initEngine();
	PlaybackController playbackController;
	Queue queue;
	playbackController.setQueue(queue);

	std::thread threadObject(PlaybackController::pullFromQueue, &playbackController, &queue);
	
	while (shouldRun) {
		char c = _getch();					//Get pressed key
		
		switch (c) {
		case 'q': {
			std::cout << "Bye" << std::endl;
			shouldRun = false;
		}
		break;
		case 'l': {
			std::cout << "Enter path: ";
			std::string path;
			std::getline(std::cin, path);
			int inputType = getInputType(path);
			std::cout << std::endl;

			if (inputType == -1) {
				std::cout << "Error 02: That is not a valid path" << std::endl;
				break;
			} else if (inputType == 0) {
				Song song(path);
				queue.pushItem(song);
				std::cout << "Song added to queue!" << std::endl;
			} else if ((inputType == 1) || (inputType == 2)) {
				Playlist playlist;
				playlist.loadSongs(path);
				std::cout << "Playlist added to queue!" << std::endl;
			}
		}
		break;
		case 'p': {

		}
		break;
		}
	}

	threadObject.join();

	engine->drop();							//Destroy engine
	return 0;
}
