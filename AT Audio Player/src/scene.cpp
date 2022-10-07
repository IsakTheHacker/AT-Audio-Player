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
	menu = new TUI::TextInputMenu(TUI::WinSize(100, 5), TUI::Coord(3, 2), "(Type path to song, playlist or folder here)");
}

std::string InputScene::run() {
	return std::get<std::string>(menu->waitForInput());
}