#pragma once
#include "DxLib.h"
#include "KeyInput.h"

struct KeyBind {
	int assign_key;		//割り振られたキー
	int check_type;		//押し方の種類 0=単押し 1=長押し 2=離した瞬間
};

class UserData
{
private:
	int master_volume;	//全体音量
	int bgm_volume;		//BGM音量
	int se_volume;		//SE音量

	KeyBind PLAYER_WALK_LEFT = { XINPUT_BUTTON_DPAD_LEFT,0 };	//左移動
	KeyBind PLAYER_WALK_RIGHT = { XINPUT_BUTTON_DPAD_RIGHT,0 }; //右移動
	KeyBind PLAYER_JUMP = { XINPUT_BUTTON_A,0 };				//ジャンプ
	KeyBind PLAYER_SWAP = { XINPUT_BUTTON_B,1 };				//交換

public:

	//ユーザーデータファイルを読み込む
	void LoadUserData();

	//ユーザーデータファイルに書き込む
	void UpdateUserData();
};

