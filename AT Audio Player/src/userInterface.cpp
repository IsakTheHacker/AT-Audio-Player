#include "userInterface.h"
#include "application.h"
#include <conterlib.hpp>

void UserInterface::drawScreen() {
	ShowScrollBar(GetConsoleWindow(), SB_VERT, false);		//Hide scrollbar
	Console::showCursor(false);

	std::string volumeString = concatString("*", playbackController->getVolume()) + concatString("o", 10 - playbackController->getVolume());
	std::string volumeNumber = std::to_string((int)playbackController->getVolume());
	if (volumeNumber.length() < 2) {
		volumeNumber = " " + volumeNumber;
	}

	std::vector queueItems = playbackController->getQueue().getItemNames();

	if (queueItems.size() > maxQueueDisplaySize) {
		queueItems[maxQueueDisplaySize - 1] = "..." + concatString(" ", Console::getScreenSizeX() - 79);
	} else if (queueItems.size() < maxQueueDisplaySize) {
		for (size_t i = queueItems.size(); i < maxQueueDisplaySize; i++) {
			queueItems.push_back(concatString(" ", Console::getScreenSizeX() - 76));
		}
	}
	for (size_t i = 0; i < queueItems.size(); i++) {
		queueItems[i] = queueItems[i] + concatString(" ", Console::getScreenSizeX() - 76 - queueItems[i].length());
	}

	TUI::Window volume = TUI::Window(6, 15, 0, 0, "Volume", TUI::WindowOptions::getNoBorderPreset());
	int y = 1;
	for (int i = 9; i >= 0; i--) {
		volume.write(1, y, concatString(volumeString[i], 3));
		y++;
	}
	volume.write(2, 12, volumeNumber);

	TUI::Window player = TUI::Window((Console::getScreenSizeX() - 6) / 2, 14, 7, 0, "Player");
	player.write(1, 0, "Song: " + playbackController->getSong().getName());
	player.write(1, 1, "Path: " + playbackController->getSong().getPath());

	TUI::Window queue = TUI::Window((Console::getScreenSizeX() - 6) / 2 - 2, 14, (Console::getScreenSizeX() - 6) / 2 + 8, 0, "Queue");
	
	//Print messages
	TUI::Window console = TUI::Window((Console::getScreenSizeX() - 6) / 2 + 7, Console::getScreenSizeY() - 14, 0, 14, "Console");
	auto now = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < messages.size(); i++) {
		if (now > messages[i].getEnd()) {
			messages.erase(messages.begin() + i);
		}
	}
	std::vector messagesCpy = messages;
	for (int i = 0; i < messagesCpy.size(); i++) {
		if (i > console.getSize().ySize - 2) break;
		console.write(1, i, messagesCpy[i].getMessage());
	}

	//Console::clear();
	volume.restore();
	player.restore();
	queue.restore();
	console.restore();
}

void UserInterface::printMessage(std::string message, unsigned int secondsOnScreen) {
	messages.push_back(Message(message, secondsOnScreen));
}
void UserInterface::pauseUIUpdater() {
	using namespace std::chrono_literals;
	paused = true;
	while (pauseCycle != true) {
		std::this_thread::sleep_for(16.67ms);
	}
}
void UserInterface::unpauseUIUpdater() {
	Console::clear();
	paused = false;
}
void UserInterface::setPlaybackController(PlaybackController& playbackController) {
	this->playbackController = &playbackController;
}
void UserInterface::updateUI(UserInterface* userInterface) {
	using namespace std::chrono_literals;
	while (Application::shouldRun) {
		Console::setCursorPos();
		userInterface->drawScreen();
		std::this_thread::sleep_for(16.67ms);
		userInterface->pauseCycle = false;
		if (userInterface->paused) {
			Console::clear();
		}
		while (userInterface->paused) {
			userInterface->pauseCycle = true;
			std::this_thread::sleep_for(16.67ms);
		}
	}
}