
#pragma once

#include <math.h>
#include "DxLib.h"

#define PAD_BUTTONS   16  // ゲームコントローラー全ボタン数
#define KEYBOARD_KEYS 256 // キーボード全キー数
#define STICK_MAX 32767.f // スティック最大値

#define PRESS   1 // 押された瞬間
#define PRESSED 2 // 押されている間
#define RELEASE 3 // 離した瞬間

#define L 0 // 左
#define R 1 // 右

#define MOVE_RIGHT 0.2
#define MOVE_LEFT -0.2
#define MOVE_UP 0.2
#define MOVE_DOWN -0.2

// スティック入力 構造体
struct PadStick {
	float x = 0; // 横軸値
	float y = 0; // 縦軸値
};

// マウスカーソル 構造体
struct MousePoint {
	int x = 0; // 横軸値
	int y = 0; // 縦軸値
};

// 入力コントローラー クラス（ゲームコントローラー / キーボード・マウス）
class InputCtrl {
private:
	static char nowBtn[PAD_BUTTONS];  // 今回のボタン入力
	static char oldBtn[PAD_BUTTONS];  // 前回のボタン入力

	static XINPUT_STATE padInput;     // ゲームコントローラー入力状態

	static PadStick padLStick;        // 左スティック
	static PadStick padRStick;        // 右スティック

	static int nowKey[KEYBOARD_KEYS]; // 今回のキーボードキー入力

	static MousePoint mousePoint;     // マウスカーソルの位置
	static int nowMouseClick[2];      // 今回のマウスクリック入力
	static int oldMouseClick[2];      // 前回のマウスクリック入力

	InputCtrl() = default;

public:
	// 入力状況の更新
	static void Update();

	// コントローラーのボタンの状態を取得
	static int GetButtonState(int);

	// コントローラーのスティックの状態を取得（構造体）
	static PadStick GetStickState(int);

	// コントローラーのスティックの傾き割合を取得（構造体）
	static PadStick GetStickRatio(int);

	// キーボードのキーの状態を取得
	static int GetKeyState(int);

	// マウスのボタンの状態を取得
	static int GetMouseState(int);

	// マウスのカーソル位置を取得（構造体）
	static MousePoint GetMouseCursor();
};