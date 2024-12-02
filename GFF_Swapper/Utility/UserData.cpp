#include "UserData.h"
#include <fstream>
#include <iostream>
#include <string>

void UserData::LoadUserData()
{
	const char* a = "Resource/Dat/UserData.txt";

	std::ifstream file(a);
	//ファイルが読み込めていたなら
	if (file)
	{
		file >> master_volume;
		file >> bgm_volume;
		file >> se_volume;
	}
}

void UserData::UpdateUserData()
{
	const char* a = "Resource/Dat/UserData.txt";

	std::ofstream file(a);
	//ファイルが読み込めていたなら
	if (file)
	{
		file << master_volume << "\n";
		file << bgm_volume << "\n";
		file << se_volume << "\n";
	}
}