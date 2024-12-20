#pragma once
#include "DxLib.h"
#include "PadInput.h"
#include <map>

#define PLAYER_INPUT_NUM (int)PlayerActionKey::P_SWAP_MOVE_RIGHT+1			//プレイヤーのキー割り当て要素数

#define	ON_BUTTON 0	//押した瞬間
#define	PRESSED 1	//押している間
#define	RELEASE 2   //離した瞬間

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
	 
	static int player_key[PLAYER_INPUT_NUM][2];	//キー割り当て情報格納		

	static int button_image[2];			//ボタン画像
public:
	//初期化
	static void Initialize();

	//ユーザーデータファイルを読み込む
	static void LoadUserData();

	//ユーザーデータファイルに書き込む
	static void UpdateUserData();

	//キー割り当てをデフォルトにリセットする
	static void ResetKeyConfig();

	//指定したキーが押されているか確認
	static bool CheckActionKey(int _player_action,int _key_state);
};

