#pragma once
#include <soloud.h>
namespace sl = SoLoud;

#include "playbackController.h"
#include "userInterface.h"
#include "queue.h"

class Application {
public:
	void run();
	Application();
	~Application();
	static bool shouldRun;					//If false: stop execution
	static sl::Soloud* engine;
private:
	PlaybackController playbackController;
	UserInterface ui;
	Queue queue;
	std::thread pullFromQueue;
	std::thread updateUI;
};