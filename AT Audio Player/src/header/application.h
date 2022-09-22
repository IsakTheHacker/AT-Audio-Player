#pragma once
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

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
#if defined(_WIN32) || defined(_WIN64)
	DWORD prevConsoleMode;
#endif
};