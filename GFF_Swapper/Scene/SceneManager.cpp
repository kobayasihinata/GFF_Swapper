#include "SceneManager.h"
#include "../Utility/DebugInformation.h"
#include "../Utility/UserData.h"

void SceneManager::Initialize()
{
	//シーンの初期化
	mScene->Initialize();
}

void SceneManager::Finalize()
{
	mScene->Finalize();
}

AbstractScene* SceneManager::Update() {

	//デバッグ表示の更新
	DebugInformation::Update();

	AbstractScene* p = mScene->Update();

	if (p != mScene && p != nullptr) {
		//old_stageにnullptrが格納されているなら、ひとつ前のシーンを削除する
		if (p->GetOldScene() == nullptr)
		{
			mScene->Finalize();
			delete mScene;
		}
		mScene = p; // 次に表示したいシーン
		//すでに生成されたシーンが戻り値として帰ってきているなら、イニシャライズをしない
		if (!mScene->GetInitFlg())
		{
			mScene->Initialize();
		}
	};
	return p;
};

void SceneManager::Draw() const {

	mScene->Draw();
};
