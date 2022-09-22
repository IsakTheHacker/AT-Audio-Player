#include "application.h"
#include <soloud_wav.h>
#include <conterlib.hpp>

int main(int argc, const char* argv[]) {
	Application* app = new Application;
	app->run();
	delete app;

	TUI::Window(20, 10, 3, 2, "test");
	//Console con;

	//SoLoud::Soloud soloud; // Engine core
	//SoLoud::Wav sample;    // One sample

	//// Initialize SoLoud (automatic back-end selection)
	//soloud.init();

	//sample.load("C:\\Virtual Insanity - Jamiroqai.mp3"); // Load a wave file
	//soloud.play(sample);        // Play it

	//while (true);
}