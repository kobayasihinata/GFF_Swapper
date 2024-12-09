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

#define L_TRIGGER (24)			//左トリガー
#define R_TRIGGER (25)			//右トリガー

//スティック
struct Stick
{
	short ThumbX;	//横軸値
	short ThumbY;	//縦軸値
};

class PadInput
{
private:
	static XINPUT_STATE Input; //パッド
	static Stick Rstick; //右スティック
	static Stick Lstick; //左スティック
	static char NowKey[BUTTONS]; //今回の入力キー
	static char OldKey[BUTTONS]; //１フレーム前の入力キー
	static bool NowStick[8];	 //今回の入力スティック
	static bool OldStick[8];	 //１フレーム前の入力スティック
	static bool NowTrigger[2];	 //今回のトリガー
	static bool OldTrigger[2];	 //１フレーム前のトリガー

	//各ボタン長押し時間測定
	static int PressingTime[R_TRIGGER];
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

	//指定したキーが指定の時間長押しされていたら、指定の間隔毎に真を返す(押した瞬間も一度だけ真を返す)
	static bool GetPressedButton(int _button, int start_time = 10, int interval = 5);

	// 現在押されているボタン、スティックの番号を返す
	// 同時に入力されている場合は各種ボタン、左スティック上下左右、
	// 右スティック上下左右の順番で若い数字の物が返される
	static int GetNowInput();
};