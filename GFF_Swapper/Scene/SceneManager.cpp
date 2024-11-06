#include "SceneManager.h"
#include "../Utility/DebugInfomation.h"

void SceneManager::Initialize()
{
	mScene->Initialize();
}

void SceneManager::Finalize()
{
	mScene->Finalize();
}

AbstractScene* SceneManager::Update() {

	DebugInfomation::Update();

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
	//デバッグ表示の更新
	DebugInfomation::Draw();
};
