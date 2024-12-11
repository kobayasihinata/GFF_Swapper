#include "UserData.h"

#include "PadInput.h"
#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <string>

int UserData::volume[] = {255,255,255};		//デフォルトの音量はMAXで

int UserData::player_key[PLAYER_INPUT_NUM][2] = {0,0};

int UserData::button_image[2];			

void UserData::Initialize()
{
	//ファイル読み込み
	LoadUserData();
	//画像読み込み
	button_image[0] = ResourceManager::SetDivGraph("Resource/Images/sozai/button1.png", 26, 7, 4, 40, 40, 0);
	button_image[1] = ResourceManager::SetDivGraph("Resource/Images/sozai/button2.png", 26, 7, 4, 40, 40, 0);

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
	}
}

void UserData::ResetKeyConfig()
{
	const char* a = "Resource/Dat/DefaultUserData.txt";

	std::ifstream file(a);
	int dammy;	//音量は変えない為、ダミーに格納する
	//ファイルが読み込めていたなら
	if (file)
	{
		file >> dammy;
		file >> dammy;
		file >> dammy;

		for (int i = 0; i < PLAYER_INPUT_NUM; i++)
		{
			file >> player_key[i][0] >> player_key[i][1];
		}
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
