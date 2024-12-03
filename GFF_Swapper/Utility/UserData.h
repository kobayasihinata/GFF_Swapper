#pragma once
#include "DxLib.h"
#include "KeyInput.h"
#include <map>

#define PLAYER_INPUT_NUM (int)PlayerActionKey::P_SWAP_MOVE_RIGHT+1			//プレイヤーのキー割り当て要素数

enum class PlayerActionKey
{
	P_WALK_LEFT = 0,		//左移動
	P_WALK_RIGHT,			//右移動
	P_JUMP,				//ジャンプ
	P_SWAP,				//交換
	P_SWAP_MOVE_UP,		//交換中のカーソル移動（上）
	P_SWAP_MOVE_DOWN,		//交換中のカーソル移動（下）
	P_SWAP_MOVE_LEFT,		//交換中のカーソル移動（左）
	P_SWAP_MOVE_RIGHT		//交換中のカーソル移動（右）
	//一番下に要素を追加するなら、PLAYER_INPUT_NUMも書き換える
};
class UserData
{
public:
	static int volume[3];					//音量 0=全体 1=BGM 2=SE
	 
	static int player_key[PLAYER_INPUT_NUM];	//キー割り当て情報格納		
	//static int PLAYER_WALK_LEFT;			//左移動
	//static int PLAYER_WALK_RIGHT;			//右移動
	//static int PLAYER_JUMP;					//ジャンプ
	//static int PLAYER_SWAP;					//交換
	//static int PLAYER_SWAP_MOVE_UP;			//交換中のカーソル移動（上）
	//static int PLAYER_SWAP_MOVE_DOWN;		//交換中のカーソル移動（下）
	//static int PLAYER_SWAP_MOVE_LEFT;		//交換中のカーソル移動（左）
	//static int PLAYER_SWAP_MOVE_RIGHT;		//交換中のカーソル移動（右）

public:
	//初期化
	static void Initialize();

	//ユーザーデータファイルを読み込む
	static void LoadUserData();

	//ユーザーデータファイルに書き込む
	static void UpdateUserData();

};

