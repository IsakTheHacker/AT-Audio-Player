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

	//Disable Escape key delay
	setenv("ESCDELAY", "0", 1);

	//Init curses
	curses::initscr();
	curses::cbreak();							//Output one character at a time, no buffering
	curses::noecho();							//Disable output of typed characters
	curses::keypad(curses::stdscr, TRUE);		//Enable special keys such as backspace, delete and the four arrow keys
	curses::nodelay(curses::stdscr, TRUE);		//Don't wait for a key to be pressed when using getch
	curses::curs_set(0);						//Hide cursor
	//signal(SIGWINCH, resizeHandler);
	
	//Init irrKlang
	initEngine(engine);

	//Other stuff
	PlaybackController playbackController;
	UserInterface ui;
	Queue queue;
	playbackController.setQueue(queue);
	ui.setPlaybackController(playbackController);
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
		case KEY_UP: {		//Volume up
			playbackController.setVolume(playbackController.getVolume() + 1);
		}
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
			InputMenu menu(5, 100);
			menu.waitForInput(2, 3, "(Type path to song, playlist or folder here)");
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
	
	// 	case 'l': {
	// 		ui.pauseUIUpdater();
	// 		std::cout << "Enter path: ";
	// 		std::string path;
	// 		std::getline(std::cin, path);
	// 		int inputType = getInputType(path);
	// 		std::cout << std::endl;

	// 		if (inputType == -1) {
	// 			std::cout << "Error 02: That is not a valid path" << std::endl;
	// 		} else if (inputType == 0) {
	// 			Song song(path);
	// 			queue.pushItem(song);
	// 			ui.printMessage("Song added to queue!");
	// 		} else if ((inputType == 1) || (inputType == 2)) {
	// 			Playlist playlist;
	// 			playlist.loadSongs(path);
	// 			std::cout << "Playlist added to queue!" << std::endl;
	// 		}
	// 		ui.unpauseUIUpdater();
	// 	}
	// 	break;
		
	// 	case 'r': {			//Rewind, douple-press => set on repeat
	// 		// auto first_click = std::chrono::high_resolution_clock::now();

	// 		// while (true) {
	// 		// 	if (Console::kbhit()) {
	// 		// 		char c = Console::getch();
	// 		// 		if (c == 'r')
	// 		// 		break;
	// 		// 	}
	// 		// 	auto second_click = std::chrono::high_resolution_clock::now();
	// 		// 	if ((second_click - first_click).count() > 400000000) {
	// 		// 		break;
	// 		// 	}
	// 		// }

	// 		// auto second_click = std::chrono::high_resolution_clock::now();
	// 		// auto duration = second_click - first_click;

	// 		// if (duration.count() < 400000000) {
	// 		// 	playbackController.switchRepeatMode();
	// 		// } else {
	// 		// 	playbackController.rewind();
	// 		// }
	// 	}
	// 	break;
	// 	}
	// }
}
