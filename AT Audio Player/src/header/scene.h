#pragma once

/*A scene is a collection of windows that make up a view for the user,
  for example the load song/playlist view or the song explorer view*/

#include "conterlib.hpp"
#include "menu.h"

class Scene {
private:
public:
	Scene();
};

class SceneMgr {
private:
	Scene* currentScene;
public:
	void switchScene(Scene*);
};

class WelcomeScene : public Scene {
private:
	SelectionMenu1d* menu = nullptr;
public:
	WelcomeScene();
	void run();
};

class InputScene : public Scene {
private:
	TUI::TextInputMenu* menu = nullptr;
public:
	InputScene();
	std::string run();
};