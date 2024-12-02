#include "SceneManager.h"
#include "../Utility/DebugInfomation.h"
#include "../Utility/UserData.h"

void SceneManager::Initialize()
{
	//ユーザー情報の格納
	UserData::Initialize();
	//シーンの初期化
	mScene->Initialize();
}

void SceneManager::Finalize()
{
	mScene->Finalize();
}

AbstractScene* SceneManager::Update() {

	//デバッグ表示の更新
	DebugInfomation::Update();

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
	DebugInfomation::Add("master", UserData::volume[0]);
	return p;
};

void SceneManager::Draw() const {

	mScene->Draw();
	//デバッグ表示の更新
	DebugInfomation::Draw();
};
