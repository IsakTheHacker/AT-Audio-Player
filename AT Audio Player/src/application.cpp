#include "application.h"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif
#include <scene.h>
#include <menu.h>

#include <thread>
#include <conterlib.hpp>
//#include "scene.h"

Application::Application() {
#if defined(_WIN32) || defined(_WIN64)
	SetConsoleOutputCP(1252);								//This makes it possible to write out characters such as едц to the command prompt
	ShowScrollBar(GetConsoleWindow(), SB_VERT, false);		//Hide scrollbar
	
	//Disable user selection
	GetConsoleMode(GetConsoleWindow(), &prevConsoleMode);
	SetConsoleMode(GetConsoleWindow(), ENABLE_EXTENDED_FLAGS | (prevConsoleMode & ~ENABLE_QUICK_EDIT_MODE));
#endif
	Console::showCursor(false);
	Console::setWindowTitle("AT Audio Player");

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
#if defined(_WIN32) || defined(_WIN64)
	ShowScrollBar(GetConsoleWindow(), SB_VERT, true);		//Show scrollbar again
	SetConsoleMode(GetConsoleWindow(), prevConsoleMode);	//Restore console mode
#endif
}

void Application::run() {
	pullFromQueue = std::thread(PlaybackController::pullFromQueue, &playbackController, &queue);
	updateUI = std::thread(UserInterface::updateUI, &ui);

	//--- Main loop BEGINS HERE ---
	while (shouldRun) {

		//Take input
		Key c = Console::readKey();
		switch (c) {
		case Key::q: {
			shouldRun = false;
		}
		break;
		case Key::CTRL_C: {
			shouldRun = false;
		}
		break;
		case Key::ARROW_UP: {		//Volume up
			playbackController.setVolume(playbackController.getVolume() + 1);
		break;
		case Key::ARROW_DOWN: {		//Volume down
			playbackController.setVolume(playbackController.getVolume() - 1);
		}
		break;
		case Key::d: {	//Skip forwards
			playbackController.seek(playbackController.getPlaytime() + 10);
		}
		break;
		case Key::a: {		//Skip backwards
			playbackController.seek(playbackController.getPlaytime() - 10);
		}
		break;
		case Key::SPACE: {
			playbackController.switchPauseMode();
		}
		break;
		case Key::r: {			//Recording room
			ui.pauseUIUpdater();
			std::vector<std::string> choices = {"Apple", "Banana", "Pear"};
			SelectionMenu1d menu(choices);
			menu.waitForSelection();
			ui.unpauseUIUpdater();
		}
		break;
		case Key::l: {			//Load song, playlist or folder
			ui.pauseUIUpdater();
			InputScene* scene = new InputScene();
			//SceneMgr.switch(scene);
			std::string path = scene->run();
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
			}
			ui.unpauseUIUpdater();
		}
		break;
		}
		}
	//--- Main loop ENDS HERE ---
	}
}

bool Application::shouldRun = true;
sl::Soloud* Application::engine = new SoLoud::Soloud;