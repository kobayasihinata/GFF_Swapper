#pragma once
#include"DxLib.h"

#define BUTTONS 16
#define STICKL_X 1		
#define STICKL_Y 2		
#define MAXL_X 32767.f  //左スティックX軸の最大値(float型)
#define MAXL_Y 32767.f  //左スティックY軸の最大値(float型)

//スティック
struct Stick
{
	short ThumbX;	//横軸値
	short ThumbY;	//縦軸値
};

class PadInput
{
private:
	static char NowKey[BUTTONS]; //今回の入力キー
	static char OldKey[BUTTONS]; //前回の入力キー
	static XINPUT_STATE Input; //パッド
	static Stick Rstick; //右スティック
	static Stick Lstick; //左スティック

public:
	//パッド情報の更新
	static void UpdateKey();

	//ボタンを押された瞬間
	static bool OnButton(int button);

	//ボタンを押してる間
	static bool OnPressed(int button);

	//ボタンを離した瞬間
	static bool OnRelease(int button);

	//スティックの傾き割合
	static float TipLeftLStick(short StickL);
};