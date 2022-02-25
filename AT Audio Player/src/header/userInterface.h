#pragma once

#include <vector>
#include <string>
#include <thread>

//Thirdparty
namespace curses {						//Include curses library in a namespace so it doesn't collide with other stuff
	#include <ncurses.h>
}

#include "playbackController.h"
#include "message.h"
#include "handyFunctions.h"
#include "globalVars.h"

class UserInterface {
private:
	bool paused = false;
	bool pauseCycle = false;
	PlaybackController* playbackController = nullptr;
	std::vector<Message> messages;
	int64_t maxQueueDisplaySize = 10;
	int64_t maxMessageQueueDisplaySize = 10;

	void drawScreen();
public:
	void printMessage(std::string message, unsigned int secondsOnScreen = 3);
	void pauseUIUpdater();
	void unpauseUIUpdater();
	void setPlaybackController(PlaybackController& playbackController);
	static void updateUI(UserInterface* userInterface);
};
