#include "scene.h"

Scene::Scene() {

}

WelcomeScene::WelcomeScene() {
	std::vector<std::string> choices = { "Resume playback where you left of" };
	menu = new SelectionMenu1d(choices);
}

void WelcomeScene::run() {
	menu->waitForSelection(0, true);
}

InputScene::InputScene() {
	menu = new InputMenu(5, 100);
}

void InputScene::run() {
	menu->waitForInput(2, 3, "(Type path to song, playlist or folder here)");
}