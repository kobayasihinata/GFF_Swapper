#pragma once
#include "../Base/Object.h"
#include "../../Scene/Camera.h"
#include <fstream>
#include <string>

class Tutorial:
	public Object
{
private:

	class Camera* in_camera;

	int tutorial_num;		//何番目のチュートリアルか
	int tutorial_time;		//チュートリアルが出ている時間
	bool tutorial_flg;		//チュートリアル中か
	bool tutorial_completed;//チュートリアルが終わったか

	Vector2D offset;		//描画ずらし用
	Vector2D offset_size;	//描画ずらし用

	std::string tutorial_text;

public:
	//コンストラクタ
	Tutorial();
	~Tutorial();

	
	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Update(ObjectManager* _manager)override;
	void Draw()const override;
	void Finalize()override;

	//ヒット時の処理
	void Hit(Object* _object)override;

	bool SearchColor(Object* ob) override {
		return false;
	}

	//チュートリアルの描画
	void DrawTutorial1()const;
	void DrawTutorial2()const;
	void DrawTutorial3()const;
	void DrawTutorial4()const;

	std::string LoadTextFile(const std::string& file_path);

	//offsetをセット
	void SetOffset();
	//flgを取得
	bool GetTutorialFlg();
};

