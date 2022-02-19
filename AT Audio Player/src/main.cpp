//#pragma comment(lib, "irrKlang.lib")		//Link with irrKlang.dll
#pragma comment(lib, "libIrrKlang.so")

//C++ std libs
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <thread>
#include <filesystem>
namespace fs = std::filesystem;

//OS-specific
//#include <conio.h>
//#include <Windows.h>

#include <conterlib.hpp>

//Thirdparty
#include <json.hpp>
#include <irrKlang.h>
using namespace irrklang;

//My includes
#include "queue.h"
#include "userInterface.h"
#include "playbackController.h"
#include "handyFunctions.h"

bool shouldRun = true;
irrklang::ISoundEngine* engine = nullptr;

//Keys that don't type a character
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ENTER 13

int main(int argc, const char* argv[]) {
	initEngine(engine);
	PlaybackController playbackController;
	UserInterface ui;
	Queue queue;

	playbackController.setQueue(queue);
	ui.setPlaybackController(playbackController);
	Console::showCursor(false);

	std::thread pullFromQueue(PlaybackController::pullFromQueue, &playbackController, &queue);
	std::thread updateUI(UserInterface::updateUI, &ui);
	
	while (shouldRun) {
		char c = Console::getch();					//Get pressed key
		
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
				ui.printMessage("Song added to queue!");
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
				if (Console::kbhit()) {
					char c = Console::getch();
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
