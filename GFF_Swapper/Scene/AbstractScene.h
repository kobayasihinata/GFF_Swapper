#pragma once
class AbstractScene
{
protected:
	AbstractScene* old_scene = nullptr;   //ひとつ前のシーン情報格納
	bool init_flg = false;				  //イニシャライズが既に呼ばれているか（途中からシーンを始めるべきか）
public:

	//デストラクタ
	virtual ~AbstractScene() {};

	//イニシャライズ
	virtual void Initialize() { init_flg = true; }

	//ファイナライズ
	virtual void Finalize() = 0;

	//描画以外の更新を実行
	virtual AbstractScene* Update() = 0;

	//描画に関することを実装
	virtual void Draw() const = 0;

	//ひとつ前のシーンの情報取得
	AbstractScene* GetOldScene()const { return old_scene; }

	//生成済みか取得
	bool GetInitFlg()const { return init_flg; }
};
