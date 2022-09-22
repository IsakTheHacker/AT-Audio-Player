#include "application.h"

#include <thread>
#include <conterlib.hpp>
//#include "scene.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define ESCAPE 27
#define ENTER 13

Application::Application() {
	Console::showCursor(false);
	//Console::showScrollbar(false);

	engine->init();
	playbackController.setQueue(queue);
	playbackController.setVolume(5);
	ui.setPlaybackController(playbackController);
}

Application::~Application() {
	pullFromQueue.join();
	updateUI.join();

	//Do cleanup
	engine->deinit();
	delete engine;

	Console::showCursor(true);
	//Console::showScrollbar(true);
}

void Application::run() {
	pullFromQueue = std::thread(PlaybackController::pullFromQueue, &playbackController, &queue);
	updateUI = std::thread(UserInterface::updateUI, &ui);

	//--- Main loop BEGINS HERE ---
	while (shouldRun) {

		//Take input
		int c = Console::getch();
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
			//SelectionMenu1d menu(choices);
			//menu.waitForSelection();
		}
		break;
		//case 'l': {			//Load song, playlist or folder
		//	InputScene* scene = new InputScene();
		//	//SceneMgr.switch(scene);
		//	scene->run();
		//}
		//break;
		}
	}
	//--- Main loop ENDS HERE ---
}

}

bool Application::shouldRun = true;
sl::Soloud* Application::engine = new SoLoud::Soloud;