#pragma once
#include "AbstractScene.h"

class SceneManager : public AbstractScene {
private:
	AbstractScene* mScene; // 現在のシーン
public:
	// コンストラクタ
	SceneManager(AbstractScene* scene) : mScene(scene) {}

	// デストラクタ
	~SceneManager() {
		delete mScene;
	};

	//イニシャライズ
	void Initialize()override;

	//ファイナライズ
	void Finalize()override;

	// 描画以外の更新を実装
	AbstractScene* Update() override;

	// 描画に関することを実装
	void Draw() const override;
};