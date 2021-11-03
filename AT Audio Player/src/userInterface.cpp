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

	std::cout << "Volume +------------------------------ ATAP ------------------------------+ Queue\n";
	std::cout << "       | Song: " << playbackController->getSong().getName() << spacesLine1 << "| " << "\n";
	std::cout << "  " << concatString(volumeString[9], 3) << "  | " << spacesLine2 << "| " << queueItems[0] << "\n";
	std::cout << "  " << concatString(volumeString[8], 3) << "  | " << spacesLine2 << "| " << queueItems[1] << "\n";
	std::cout << "  " << concatString(volumeString[7], 3) << "  | " << spacesLine2 << "| " << queueItems[2] << "\n";
	std::cout << "  " << concatString(volumeString[6], 3) << "  | " << spacesLine2 << "| " << queueItems[3] << "\n";
	std::cout << "  " << concatString(volumeString[5], 3) << "  | " << spacesLine2 << "| " << queueItems[4] << "\n";
	std::cout << "  " << concatString(volumeString[4], 3) << "  | " << spacesLine2 << "| " << queueItems[5] << "\n";
	std::cout << "  " << concatString(volumeString[3], 3) << "  | " << spacesLine2 << "| " << queueItems[6] << "\n";
	std::cout << "  " << concatString(volumeString[2], 3) << "  | " << spacesLine2 << "| " << queueItems[7] << "\n";
	std::cout << "  " << concatString(volumeString[1], 3) << "  | " << spacesLine2 << "| " << queueItems[8] << "\n";
	std::cout << "  " << concatString(volumeString[0], 3) << "  | " << spacesLine2 << "| " << queueItems[9] << "\n";
	std::cout << "       | " << spacesLine2 << "| " << "\n";
	std::cout << "   " << volumeNumber << "  +------------------------------------------------------------------+" << "\n\n";


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

	std::cout << "       " << messagesCpy[0].getMessage() << "\n";
	std::cout << "       " << messagesCpy[1].getMessage() << "\n";
	std::cout << "       " << messagesCpy[2].getMessage() << "\n";
	std::cout << "       " << messagesCpy[3].getMessage() << "\n";
	std::cout << "       " << messagesCpy[4].getMessage() << "\n";
	std::cout << "       " << messagesCpy[5].getMessage() << "\n";
	std::cout << "       " << messagesCpy[6].getMessage() << "\n";
	std::cout << "       " << messagesCpy[7].getMessage() << "\n";
	std::cout << "       " << messagesCpy[8].getMessage() << "\n";
	std::cout << "       " << messagesCpy[9].getMessage() << "\n";
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