#pragma once
#include"DxLib.h"

#define KEY_MAX 256
#define MOUSE_MAX 256

//マウスカーソル
struct CURSOR
{
	int x;	//横軸値
	int y;	//縦軸値
};

//キー入力
class KeyInput
{
private:
	static int NowKey[KEY_MAX];		//今回の入力キー
	static int OldKey[KEY_MAX];		//前回の入力キー
	static int NowMouse[MOUSE_MAX]; //今回の入力マウスボタン
	static int OldMouse[MOUSE_MAX]; //前回の入力マウスボタン
	static CURSOR Cursor; //カーソル
public:
	//パッド情報の更新
	static void UpdateKey();

	//ボタンを押した瞬間
	static bool OnKey(int key);

	//ボタンを押した瞬間
	static bool OnPresed(int key);

	//ボタンを押した瞬間
	static bool OnRelease(int key);

	//マウスボタンを押された瞬間
	static bool OnMouse(int mouse);

	//マウスボタンを押してる間
	static bool OnPressedMouse(int mouse);

	//マウスボタンを離した瞬間
	static bool OnReleaseMouse(int mouse);

	//マウスカーソルの位置を返す
	static CURSOR GetMouseCursor();
};

