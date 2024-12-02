#pragma once
#include "DxLib.h"
#include "KeyInput.h"

class UserData
{
public:
	static int volume[3];				//音量 0=全体 1=BGM 2=SE
	 
	static int PLAYER_WALK_LEFT;		//左移動
	static int PLAYER_WALK_RIGHT;		//右移動
	static int PLAYER_JUMP;				//ジャンプ
	static int PLAYER_SWAP;				//交換
	static int PLAYER_SWAP_MOVE_UP;		//交換中のカーソル移動（上）
	static int PLAYER_SWAP_MOVE_DOWN;	//交換中のカーソル移動（下）
	static int PLAYER_SWAP_MOVE_LEFT;	//交換中のカーソル移動（左）
	static int PLAYER_SWAP_MOVE_RIGHT;	//交換中のカーソル移動（右）

public:
	//初期化
	static void Initialize();

	//ユーザーデータファイルを読み込む
	static void LoadUserData();

	//ユーザーデータファイルに書き込む
	static void UpdateUserData();

};

