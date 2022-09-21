#include "userInterface.h"

void UserInterface::drawScreen() {
	std::string spacesLine1 = concatString(" ", 59 - playbackController->getSong().getName().length());
	std::string spacesLine2 = concatString(" ", 65);

	std::string volumeString = concatString("*", playbackController->getVolume()) + concatString("o", 10 - playbackController->getVolume());
	std::string volumeNumber = std::to_string((int)playbackController->getVolume());
	if (volumeNumber.length() < 2) {
		volumeNumber = " " + volumeNumber;
	}

	std::vector queueItems = playbackController->getQueue().getItemNames();

	if (queueItems.size() > maxQueueDisplaySize) {
		queueItems[maxQueueDisplaySize - 1] = "..." + concatString(" ", Console::getXSize() - 79);
	} else if (queueItems.size() < maxQueueDisplaySize) {
		for (size_t i = queueItems.size(); i < maxQueueDisplaySize; i++) {
			queueItems.push_back(concatString(" ", Console::getXSize() - 76));
		}
	}
	for (size_t i = 0; i < queueItems.size(); i++) {
		queueItems[i] = queueItems[i] + concatString(" ", Console::getXSize() - 76 - queueItems[i].length());
	}

	curses::printw("Volume +------------------------------ ATAP ------------------------------+ Queue\n");
	curses::printw("       | Song: %s%s| \n", playbackController->getSong().getName().c_str(), spacesLine1.c_str());
	curses::printw("  %s  | %s| %s\n", concatString(volumeString[9], 3).c_str(), spacesLine2.c_str(), queueItems[0].c_str());
	curses::printw("  %s  | %s| %s\n", concatString(volumeString[8], 3).c_str(), spacesLine2.c_str(), queueItems[1].c_str());
	curses::printw("  %s  | %s| %s\n", concatString(volumeString[7], 3).c_str(), spacesLine2.c_str(), queueItems[2].c_str());
	curses::printw("  %s  | %s| %s\n", concatString(volumeString[6], 3).c_str(), spacesLine2.c_str(), queueItems[3].c_str());
	curses::printw("  %s  | %s| %s\n", concatString(volumeString[5], 3).c_str(), spacesLine2.c_str(), queueItems[4].c_str());
	curses::printw("  %s  | %s| %s\n", concatString(volumeString[4], 3).c_str(), spacesLine2.c_str(), queueItems[5].c_str());
	curses::printw("  %s  | %s| %s\n", concatString(volumeString[3], 3).c_str(), spacesLine2.c_str(), queueItems[6].c_str());
	curses::printw("  %s  | %s| %s\n", concatString(volumeString[2], 3).c_str(), spacesLine2.c_str(), queueItems[7].c_str());
	curses::printw("  %s  | %s| %s\n", concatString(volumeString[1], 3).c_str(), spacesLine2.c_str(), queueItems[8].c_str());
	curses::printw("  %s  | %s| %s\n", concatString(volumeString[0], 3).c_str(), spacesLine2.c_str(), queueItems[9].c_str());
	curses::printw("       | %s| \n", spacesLine2.c_str());
	curses::printw("   %s  +------------------------------------------------------------------+\n\n", volumeNumber.c_str());


	//Print messages
	auto now = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < messages.size(); i++) {
		if (now > messages[i].getEnd()) {
			messages.erase(messages.begin() + i);
		}
	}
	std::vector messagesCpy = messages;
	if (messagesCpy.size() > maxMessageQueueDisplaySize) {
		messagesCpy.erase(messagesCpy.begin());
		messages.erase(messages.begin());
	} else if (messagesCpy.size() < maxMessageQueueDisplaySize) {
		for (size_t i = messagesCpy.size(); i < maxQueueDisplaySize; i++) {
			messagesCpy.push_back(Message(concatString(" ", Console::getXSize() - 7), 0));
		}
	}

	// curses::printw("       " << messagesCpy[0].getMessage() << "\n";
	// curses::printw("       " << messagesCpy[1].getMessage() << "\n";
	// curses::printw("       " << messagesCpy[2].getMessage() << "\n";
	// curses::printw("       " << messagesCpy[3].getMessage() << "\n";
	// curses::printw("       " << messagesCpy[4].getMessage() << "\n";
	// curses::printw("       " << messagesCpy[5].getMessage() << "\n";
	// curses::printw("       " << messagesCpy[6].getMessage() << "\n";
	// curses::printw("       " << messagesCpy[7].getMessage() << "\n";
	// curses::printw("       " << messagesCpy[8].getMessage() << "\n";
	// curses::printw("       " << messagesCpy[9].getMessage() << "\n";
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
	paused = false;
}
void UserInterface::setPlaybackController(PlaybackController& playbackController) {
	this->playbackController = &playbackController;
}
void UserInterface::updateUI(UserInterface* userInterface) {
	using namespace std::chrono_literals;
	while (shouldRun) {
		curses::move(0, 0);
		userInterface->drawScreen();
		std::this_thread::sleep_for(16.67ms);
		userInterface->pauseCycle = false;
		if (userInterface->paused) {
			curses::clear();
		}
		while (userInterface->paused) {
			userInterface->pauseCycle = true;
			std::this_thread::sleep_for(16.67ms);
		}
	}
}