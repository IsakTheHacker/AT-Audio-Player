#include "application.h"

int main(int argc, const char* argv[]) {
	Application* app = new Application;
	app->run();
	delete app;
	return EXIT_SUCCESS;
}