#pragma once
#include "../Base/Object.h"
#include "../../Scene/Camera.h"
#include "../../Utility/UserData.h"
#include "../../Scene/Option.h"

#include <vector>   // std::vector を使うため
#include <string>   // std::string を使うため
#include <fstream>  // std::ifstream を使うため
#include <map>

class Tutorial:
	public Object
{
private:

	class Camera* in_camera;

	int keyCode;
	std::string keyName[12];

	int tutorial_num;		//何番目のチュートリアルか
	int tutorial_time;		//チュートリアルが出ている時間
	bool tutorial_flg;		//チュートリアル中か
	bool tutorial_completed;//チュートリアルが終わったか

	Vector2D offset;		//描画ずらし用
	Vector2D offset_size;	//描画ずらし用

	std::string tutorial_text;
	std::vector<std::string> tutorial_text_list;

	std::map<int, std::string> keyMap = {
		{XINPUT_BUTTON_A, "A"},
		{XINPUT_BUTTON_B, "B"},
		{XINPUT_BUTTON_X, "X"},
		{XINPUT_BUTTON_Y, "Y"},
		{18, "L"},
		{19, "R"},
		{XINPUT_BUTTON_DPAD_UP, "↑"},
		{XINPUT_BUTTON_DPAD_LEFT, "←"},
		{XINPUT_BUTTON_DPAD_RIGHT, "→"},
		{XINPUT_BUTTON_DPAD_DOWN, "↓"},

	};

	//描画用
	bool button_draw;
	int frame;
	int thumb_offset;


public:
	//コンストラクタ
	Tutorial(int _num);
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
	void DrawTutorial()const;

	std::string LoadTextFile(const std::string& file_path);
	//void LoadTextFile(const std::string& file_path);

	//offsetをセット
	void SetOffset();
	//flgを取得
	bool GetTutorialFlg();

	std::string GetKeyName(int keyCode);

	void DrawButton()const;
};

