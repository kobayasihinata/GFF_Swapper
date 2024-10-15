
#include "sceneManager.h"

Scene* SceneManager::update() {
	Scene* nextScene = nowScene->update();
	if (nextScene != nowScene) {
		delete nowScene;
		nowScene = nextScene;
	};
	return nextScene;
};

void SceneManager::draw() const {
	nowScene->draw();
};