#include "SceneManager.h"

void SceneManager::Initialize()
{
	mScene->Initialize();
}

void SceneManager::Finalize()
{
	mScene->Finalize();
}

AbstractScene* SceneManager::Update() {
	AbstractScene* p = mScene->Update();

	if (p != mScene && p != nullptr) {
		mScene->Finalize();
		delete mScene;
		mScene = p; // 次に表示したいシーン
		mScene->Initialize();
	};

	return p;
};

void SceneManager::Draw() const {
	mScene->Draw();
};
