#pragma once
#include"DxLib.h"

#define BUTTONS 16
#define STICKL_X 1		
#define STICKL_Y 2		
#define MAXL_X 32767.f  //左スティックX軸の最大値(float型)
#define MAXL_Y 32767.f  //左スティックY軸の最大値(float型)

//キー割り当て用
#define L_STICK_UP (16)			//左スティックの上
#define L_STICK_DOWN (17)		//左スティックの下
#define L_STICK_LEFT (18)		//左スティックの左
#define L_STICK_RIGHT (19)		//左スティックの右
#define R_STICK_UP (20)			//右スティックの上
#define R_STICK_DOWN (21)		//右スティックの下
#define R_STICK_LEFT (22)		//右スティックの左
#define R_STICK_RIGHT (23)		//右スティックの右

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
	static char OldKey[BUTTONS]; //１フレーム前の入力キー
	static bool NowStick[8];	 //今回の入力スティック
	static bool OldStick[8];	 //１フレーム前の入力スティック
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

	//スティックの傾き割合（左）
	static float TipLStick(short StickL);

	//スティックの傾き割合（右）
	static float TipRStick(short StickR);

	//スティックの傾きをboolで返す
	static bool GetStick(int _assign_key);

	/////スティックが指定した割合より傾いていたら真を返す
	//static bool CheckIncStick(bool stickL_or_stickR, short x_or_y, float rat);
};