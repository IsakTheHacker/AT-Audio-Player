//IsakTheHacker 19 februari 2022  -  AT Audio Player

//C++ std libs
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <thread>
#include <filesystem>
namespace fs = std::filesystem;

//Thirdparty
#include <json.hpp>
#include <irrKlang.h>
namespace ik = irrklang;
namespace curses {						//Include curses library in a namespace so it doesn't collide with other stuff
	#include <ncurses.h>
}

//My includes
#include "queue.h"
#include "userInterface.h"
#include "playbackController.h"
#include "handyFunctions.h"
#include "menu.h"
#include "scene.h"
#include "song.h"

//Global vars
bool shouldRun = true;					//If false: stop execution
ik::ISoundEngine* engine = nullptr;		//irrKlang engine instance

// #include <signal.h>
// void* resizeHandler(int sig) {
// 	using namespace curses;
// 	int nh, nw;
// 	getmaxyx(stdscr, nh, nw); 			//Get the new screen size
// }

int main(int argc, const char* argv[]) {
	initEngine();
	PlaybackController playbackController;
	UserInterface ui;
	Queue queue;
	playbackController.setQueue(queue);
	ui.setPlaybackController(playbackController);
	Console::showCursor(false);
	Console::showScrollbar(false);

	std::thread pullFromQueue(PlaybackController::pullFromQueue, &playbackController, &queue);
	std::thread updateUI(UserInterface::updateUI, &ui);

	//--- Main loop BEGINS HERE ---
	while (shouldRun) {

		//Take input
		int c = curses::wgetch(curses::stdscr);
		switch (c) {
		case 'q': {
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
				ui.printMessage("Error 02: That is not a valid path");
			} else if (inputType == 0) {
				Song song(path);
				queue.pushItem(song);
				ui.printMessage("Song added to queue!");
			} else if ((inputType == 1) || (inputType == 2)) {
				Playlist playlist;
				playlist.loadSongs(path);
				queue.pushItem(playlist);
				ui.printMessage("Playlist added to queue!");

				while (true) {
					std::cout << playlist.popFront().getName() << std::endl;
				}
			}
			ui.unpauseUIUpdater();
		}
		break;
		case KEY_UP: {		//Volume up
			playbackController.setVolume(playbackController.getVolume() + 1);
		break;
		case KEY_DOWN: {	//Volume down
			playbackController.setVolume(playbackController.getVolume() - 1);
		}
		break;
		case KEY_RIGHT: {	//Skip forwards
			
		}
		break;
		case KEY_LEFT: {	//Skip backwards
			
		}
		break;
		case 'r': {			//Recording room
			std::vector<std::string> choices = {"Apple", "Banana", "Pear"};
			SelectionMenu1d menu(choices);
			menu.waitForSelection();
		}
		break;
		case 'l': {			//Load song, playlist or folder
			InputScene* scene = new InputScene();
			//SceneMgr.switch(scene);
			scene->run();
		}
		break;
		}
		//handleKeyboardInput();

		//Do processing

		//Render screen
		curses::refresh();
	}
	//--- Main loop ENDS HERE ---

	pullFromQueue.join();
	updateUI.join();
	//std::cout << "irrKlang ended with status code: " << engine->drop() << std::endl;		//Causes segmentation fault for some reason
	curses::curs_set(1);				//Show cursor again
	std::cout << "Curses ended with status code: " << curses::endwin() << std::endl;
	return 0;
}
