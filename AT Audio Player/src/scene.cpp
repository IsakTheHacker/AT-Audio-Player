#include "scene.h"

Scene::Scene() {
	
}

InputScene::InputScene() {
	menu = new InputMenu(5, 100);
}

void InputScene::run() {
	menu->waitForInput(2, 3, "(Type path to song, playlist or folder here)");
}