#pragma once

#include "scene.h"

// シーンマネージャー クラス
class SceneManager : public Scene {
private:
	Scene* nowScene;

public:
	// コンストラクタ
	SceneManager(Scene* scene) : nowScene(scene) {};

	// デストラクタ
	~SceneManager() {
		delete nowScene;
	};

	// 更新
	Scene* update() override;

	// 描画
	void draw() const override;
};