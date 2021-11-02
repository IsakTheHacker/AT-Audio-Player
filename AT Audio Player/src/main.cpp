#pragma comment(lib, "irrKlang.lib")		//Link with irrKlang.dll

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

//OS-specific
#include <Windows.h>

//Thirdparty
#include <json.hpp>
#include <irrKlang.h>
using namespace irrklang;

//Keys that don't type a character
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ENTER 13

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

std::string concatString(std::string subStr, int times) {
	std::string str;
	for (size_t i = 0; i < times; i++) {
		str += subStr;
	}
	return str;
}
std::string concatString(char subStr, int times) {
	std::string str;
	for (size_t i = 0; i < times; i++) {
		str += subStr;
	}
	return str;
}

//Console functions
void clearScreen() {
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}
void setConsoleCursorPos(int x = 0, int y = 0) {
	HANDLE handle;
	COORD coordinates;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	coordinates.X = x;
	coordinates.Y = y;
	SetConsoleCursorPosition(handle, coordinates);
}
void showConsoleCursor(const bool& show) {
#if defined(_WIN32) || defined(_WIN64)										//Windows
	static const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(handle, &cci);
	cci.bVisible = show;													//Show/hide cursor
	SetConsoleCursorInfo(handle, &cci);
#elif defined(__linux__)													//Linux
	std::cout << (show ? "\033[?25h" : "\033[?25l");						//Show/hide cursor
#endif
}

class Song {
private:
	std::string path = "Not set";
	std::string name = "None";
	ISoundSource* song;
public:
	Song() { }
	Song(std::string path) {
		song = engine->addSoundSourceFromFile(path.c_str(), ESM_AUTO_DETECT, true);		//Third parameter set to true == preload
		if (!song) {
			song = engine->getSoundSource(path.c_str());
		}
		fs::path songPath(path);
		name = songPath.filename().string();
		this->path = path;
	}
	std::string getPath() { return path; }
	std::string getName() { return name; }
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
			sound = engine->play2D(loadedItem.getSong().getSource(), false, false, true);
		} else if (loadedItem.getContents() == 1) {		//Playlist
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
	void switchPauseMode() {
		if (!sound) return;
		if (sound->getIsPaused() == true) {		//Sound is paused, unpause
			unpause();
		} else {
			pause();
		}
	}
	bool isPlaying() {
		if (!sound) return false;
		if (engine->isCurrentlyPlaying(sound->getSoundSource())) {
			return true;
		} else {
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
		} else {
			sound->setIsLooped(true);
			std::cout << "The song has been set on repeat!" << std::endl;
		}
	}
	void setVolume(double volume) {
		if (!sound) return;
		if (volume > 10 || volume < 0) return;
		sound->setVolume(volume / 10);
	}
	double getVolume() {
		if (!sound) return 5;
		return sound->getVolume() * 10;
	}
	Song getSong() {
		if (!sound) {
			return Song();
		}
		if (loadedItem.getContents() == 0) {			//Song
			return loadedItem.getSong();
		} else if (loadedItem.getContents() == 1) {		//Playlist
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

class UserInterface {
private:
	bool paused = false;
	bool pauseCycle = false;
	PlaybackController* playbackController;
	std::queue<std::string> messageQueue;
	
	void drawScreen() {
		std::string spacesLine1 = concatString(" ", 59 - playbackController->getSong().getName().length());
		std::string spacesLine2 = concatString(" ", 65);

		std::string volumeString = concatString("*", playbackController->getVolume()) + concatString("o", 10 - playbackController->getVolume());
		
		std::cout << "Volume +------------------------------ ATAP ------------------------------+ Queue\n";
		std::cout << "       | Song: " << playbackController->getSong().getName() << spacesLine1 << "| "    << "\n";
		std::cout << "  " << concatString(volumeString[9], 3) << "  | " << spacesLine2 << "| "    << "\n";
		std::cout << "  " << concatString(volumeString[8], 3) << "  | " << spacesLine2 << "| "    << "\n";
		std::cout << "  " << concatString(volumeString[7], 3) << "  | " << spacesLine2 << "| "    << "\n";
		std::cout << "  " << concatString(volumeString[6], 3) << "  | " << spacesLine2 << "| "    << "\n";
		std::cout << "  " << concatString(volumeString[5], 3) << "  | " << spacesLine2 << "| "    << "\n";
		std::cout << "  " << concatString(volumeString[4], 3) << "  | " << spacesLine2 << "| "    << "\n";
		std::cout << "  " << concatString(volumeString[3], 3) << "  | " << spacesLine2 << "| "    << "\n";
		std::cout << "  " << concatString(volumeString[2], 3) << "  | " << spacesLine2 << "| "    << "\n";
		std::cout << "  " << concatString(volumeString[1], 3) << "  | " << spacesLine2 << "| "    << "\n";
		std::cout << "  " << concatString(volumeString[0], 3) << "  | " << spacesLine2 << "| "    << "\n";
	}
public:
	void printMessage(std::string message, unsigned int secondsOnScreen = 3) {
		if (messageQueue.size() > 4) {
			messageQueue.pop();
		}
		messageQueue.push(message);
	}
	void pauseUIUpdater() {
		using namespace std::chrono_literals;
		paused = true;
		while (pauseCycle != true) {
			std::this_thread::sleep_for(16.67ms);
		}
	}
	void unpauseUIUpdater() {
		paused = false;
	}
	void setPlaybackController(PlaybackController& playbackController) {
		this->playbackController = &playbackController;
	}
	static void updateUI(UserInterface* userInterface) {
		using namespace std::chrono_literals;
		while (shouldRun) {
			setConsoleCursorPos();
			userInterface->drawScreen();
			std::this_thread::sleep_for(16.67ms);
			userInterface->pauseCycle = false;
			if (userInterface->paused) {
				clearScreen();
			}
			while (userInterface->paused) {
				userInterface->pauseCycle = true;
				std::this_thread::sleep_for(16.67ms);
			}
		}
	}
};

int main(int argc, const char* argv[]) {
	initEngine();
	PlaybackController playbackController;
	UserInterface ui;
	Queue queue;

	playbackController.setQueue(queue);
	ui.setPlaybackController(playbackController);
	showConsoleCursor(false);

	std::thread pullFromQueue(PlaybackController::pullFromQueue, &playbackController, &queue);
	std::thread updateUI(UserInterface::updateUI, &ui);
	
	while (shouldRun) {
		char c = _getch();					//Get pressed key
		
		switch (c) {
		case 'q': {
			std::cout << "Bye" << std::endl;
			shouldRun = false;
		}
		break;
		case 'l': {
			ui.pauseUIUpdater();
			std::cout << "Enter path: ";
			std::string path;
			std::getline(std::cin, path);
			int inputType = getInputType(path);
			std::cout << std::endl;

			if (inputType == -1) {
				std::cout << "Error 02: That is not a valid path" << std::endl;
			} else if (inputType == 0) {
				Song song(path);
				queue.pushItem(song);
				std::cout << "Song added to queue!" << std::endl;
			} else if ((inputType == 1) || (inputType == 2)) {
				Playlist playlist;
				playlist.loadSongs(path);
				std::cout << "Playlist added to queue!" << std::endl;
			}
			ui.unpauseUIUpdater();
		}
		break;
		case 'p': {
			playbackController.switchPauseMode();
		}
		break;
		case 'n': {
			playbackController.skip();
		}
		break;
		case 'r': {			//Rewind, douple-press => set on repeat
			auto first_click = std::chrono::high_resolution_clock::now();

			while (true) {
				if (_kbhit()) {
					char c = _getch();
					if (c == 'r')
					break;
				}
				auto second_click = std::chrono::high_resolution_clock::now();
				if ((second_click - first_click).count() > 400000000) {
					break;
				}
			}

			auto second_click = std::chrono::high_resolution_clock::now();
			auto duration = second_click - first_click;

			if (duration.count() < 400000000) {
				playbackController.switchRepeatMode();
			} else {
				playbackController.rewind();
			}
		}
		break;
		case KEY_UP: {		//Volume up
			playbackController.setVolume(playbackController.getVolume() + 1);
		}
		break;
		case KEY_DOWN: {	//Volume down
			playbackController.setVolume(playbackController.getVolume() - 1);
		}
		break;
		}
	}

	pullFromQueue.join();
	updateUI.join();

	engine->drop();							//Destroy engine
	return 0;
}
