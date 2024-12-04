#include "UserData.h"

#include "PadInput.h"

#include <fstream>
#include <iostream>
#include <string>

int UserData::volume[] = {255,255,255};		//デフォルトの音量はMAXで

int UserData::player_key[PLAYER_INPUT_NUM][2] = {0,0};
//int UserData::PLAYER_WALK_LEFT = L_STICK_UP;				 //左移動
//int UserData::PLAYER_WALK_RIGHT = XINPUT_BUTTON_DPAD_RIGHT;  //右移動
//int UserData::PLAYER_JUMP = XINPUT_BUTTON_A;				 //ジャンプ
//int UserData::PLAYER_SWAP = XINPUT_BUTTON_B;				 //交換
//int UserData::PLAYER_SWAP_MOVE_UP;							 //交換中のカーソル移動（上）
//int UserData::PLAYER_SWAP_MOVE_DOWN;						 //交換中のカーソル移動（下）
//int UserData::PLAYER_SWAP_MOVE_LEFT;						 //交換中のカーソル移動（左）
//int UserData::PLAYER_SWAP_MOVE_RIGHT;						 //交換中のカーソル移動（右）

void UserData::Initialize()
{
	//ファイル読み込み
	LoadUserData();
}

void UserData::LoadUserData()
{
	const char* a = "Resource/Dat/UserData.txt";

	std::ifstream file(a);
	//ファイルが読み込めていたなら
	if (file)
	{
		file >> volume[0];
		file >> volume[1];
		file >> volume[2];

		for (int i = 0; i < PLAYER_INPUT_NUM; i++)
		{
			file >> player_key[i][0] >> player_key[i][1];
		}
		//file >> PLAYER_WALK_LEFT;
		//file >> PLAYER_WALK_RIGHT;
		//file >> PLAYER_JUMP;
		//file >> PLAYER_SWAP;
		//file >> PLAYER_SWAP_MOVE_UP;
		//file >> PLAYER_SWAP_MOVE_DOWN;
		//file >> PLAYER_SWAP_MOVE_LEFT;
		//file >> PLAYER_SWAP_MOVE_RIGHT;
	}
}

void UserData::UpdateUserData()
{
	const char* a = "Resource/Dat/UserData.txt";

	std::ofstream file(a);
	//ファイルが読み込めていたなら
	if (file)
	{
		file << volume[0] << "\n";
		file << volume[1] << "\n";
		file << volume[2] << "\n";
		for (int i = 0; i < PLAYER_INPUT_NUM; i++)
		{
			file << player_key[i][0] << " "<<player_key[i][1] << "\n";
		}
	/*	file << PLAYER_WALK_LEFT << "\n";
		file << PLAYER_WALK_RIGHT << "\n";
		file << PLAYER_JUMP << "\n";
		file << PLAYER_SWAP << "\n";
		file << PLAYER_SWAP_MOVE_UP << "\n";
		file << PLAYER_SWAP_MOVE_DOWN << "\n";
		file << PLAYER_SWAP_MOVE_LEFT << "\n";
		file << PLAYER_SWAP_MOVE_RIGHT << "\n";*/
	}
}

bool UserData::CheckActionKey(int _player_action, int _key_state)
{
	//指定したアクションに割り当てられているキーの内、いずれかが指定の状態を満たしていたら、真を返す
	switch (_key_state)
	{
		//押した瞬間
	case ON_BUTTON:
		return (PadInput::OnButton(player_key[_player_action][0]) || PadInput::OnButton(player_key[_player_action][1]));
		break;

		//押している間
	case PRESSED:
		return (PadInput::OnPressed(player_key[_player_action][0]) || PadInput::OnPressed(player_key[_player_action][1]));
		break;

		//離した瞬間
	case RELEASE:
		return (PadInput::OnRelease(player_key[_player_action][0]) || PadInput::OnRelease(player_key[_player_action][1]));
		break;

	default:
		return false;
		break;
	}
	return false;
}
