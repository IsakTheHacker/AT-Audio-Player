#pragma once

/*A scene is a collection of windows that make up a view for the user,
  for example the load song/playlist view or the song explorer view*/

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

class InputScene : public Scene {
private:
	InputMenu* menu = nullptr;
public:
	InputScene();
	void run();
};