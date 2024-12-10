#include "Tutorial.h"
#include "../../Scene/GameMain.h"
#include "../../Utility/DebugInfomation.h"
#include "../../Utility/PadInput.h"


Tutorial::Tutorial(int _num): tutorial_time(0),tutorial_flg(false), tutorial_completed(false)
{
	in_camera = Camera::Get();
	tutorial_num = _num - 23;
}

Tutorial::~Tutorial()
{
}

void Tutorial::Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)
{
	location = _location;
	erea = _erea;

	color = _color_data;

	object_pos = _object_pos;

	type = TUTORIAL;

	// チュートリアル終了フラグ
	tutorial_completed = false;

	//tutorial_text = LoadTextFile("Resource/Dat/TutorialText/tuto1.txt"); // ファイル名を指定
	//LoadTextFile("Resource/Dat/TutorialText.txt"); //

	for (int i = 0; i < PLAYER_INPUT_NUM; i++)
	{
		keyName[i] = GetKeyName(UserData::player_key[i][0]);
	}

}

void Tutorial::Update(ObjectManager* _manager)
{
	frame++;
	SetOffset();

	// チュートリアルが進行中の場合
	if (tutorial_flg)
	{
		tutorial_time++;
		// 指定時間経過でチュートリアル終了
		if (/*PadInput::OnButton(XINPUT_BUTTON_RIGHT_SHOULDER)*/tutorial_time >= 180)
		{
			tutorial_flg = false;
			tutorial_completed = true; // 完了フラグを設定
			tutorial_time = 0;;
		}

		switch (tutorial_num)
		{
		case 1:
			tutorial_text = LoadTextFile("Resource/Dat/TutorialText/tuto1.txt"); // ファイル名を指定
			break;
		case 2:
			tutorial_text = LoadTextFile("Resource/Dat/TutorialText/tuto2.txt"); // ファイル名を指定
			break;
		case 3:
			tutorial_text = LoadTextFile("Resource/Dat/TutorialText/tuto3.txt"); // ファイル名を指定
			break;
		case 4:
			//DrawTutorial4();
			break;
		}
	}

	
	/*if (frame % 30 == 0)
	{
		button_draw = !button_draw;

	}*/
	if (frame % 3 == 0)
	{
		thumb_offset++;
		if (thumb_offset > 13)
		{
			thumb_offset = 0;
		}
	}
	if (frame > 60)
	{
		frame = 0;
	}
}

void Tutorial::Draw() const
{
	//DrawBoxAA(local_location.x, local_location.y - 550, local_location.x + erea.x, local_location.y + erea.y, GetColor(255, 255, 255), FALSE);
	//DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);
	// DrawBoxAA(local_location.x + 200, local_location.y - 400, local_location.x + erea.x + 400, local_location.y + 70, GetColor(0, 0, 0), TRUE);

	/*if (is_tutorial) {
		DrawFormatString(0, 400, 0xFFFFFF, "TUTORIAL",TRUE);
	}*/
	
	//DrawBoxAA(local_location.x, local_location.y - 500, local_location.x + erea.x, local_location.y + erea.y, GetColor(255, 255, 255), FALSE);


	// 現在のチュートリアル番号に対応するテキストを取得
	/*if (tutorial_num > 0 && tutorial_num <= tutorial_text_list.size()) {
		DrawFormatString(offset.x + 20, offset.y + 30, GetColor(255, 255, 255), tutorial_text_list[tutorial_num - 1].c_str());
	}*/

	if (tutorial_flg)
	{
		DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);
		// 描画
		//DrawFormatString(offset.x + 20, offset.y + 30, GetColor(255, 255, 255), "%s",keyName[1].c_str());

		//DrawFormatString(offset.x + 20, offset.y + 30, GetColor(255, 255, 255), "%s + %s",keyName[3].c_str(), keyName[0].c_str());
		DrawButton();
	}

	//DrawTutorial();
	DebugInfomation::Add("tuto_loc", location.x);
	DebugInfomation::Add("tuto_num", tutorial_num);
}

void Tutorial::Finalize()
{
}

void Tutorial::Hit(Object* _object)
{
	/*if (_object->GetObjectType() == PLAYER)
	{
		is_tutorial = true;
	}*/

	if (_object->GetObjectType() == PLAYER /*&& !tutorial_completed*/)
	{
		tutorial_flg = true;
		if (location.x > 2000)
		{
			tutorial_num = 2;
		}
	}
}

void Tutorial::DrawTutorial() const
{
	DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);
	// チュートリアルテキストを描画
	DrawFormatString(offset.x + 20, offset.y + 30, GetColor(255, 255, 255), tutorial_text.c_str());
}

void Tutorial::DrawButton() const
{
	//ボタンイメージ描画
	SetFontSize(16);
	if (!button_draw)
	{
		DrawCircleAA(offset.x + 75, offset.y + 50, 20, 100, 0xff0000, FALSE);
		DrawFormatString(offset.x + 75 - 3, offset.y + 43, 0xff0000, "%s", keyName[3].c_str());
	}
	/*else
	{
		DrawCircleAA(offset.x + 50, offset.y + 45, 20, 100, 0xff0000, TRUE);
		DrawCircleAA(offset.x + 50, offset.y + 50, 20, 100, 0xff0000, TRUE);
		DrawBoxAA(offset.x + 50, offset.y + 45, offset.x + 50, offset.y + 50, 0xff0000, TRUE);
		DrawStringF(offset.x + 50 - 3, offset.y + 38, "B", 0x000000);
	}*/

	DrawString(offset.x + 116, offset.y + 43, "&", 0xFFFFFF);

	//スティック描画
	DrawCircleAA(offset.x + 170, offset.y + 50, 23, 20, 0xaaaaaa, false);
	DrawCircleAA(offset.x + 170, offset.y + 50 + thumb_offset, 18, 20, 0xaaaaaa, true);
	DrawFormatString(offset.x + 166, offset.y + 43 + thumb_offset, 0xFFFFFF, "%s", keyName[0].c_str());

}


std::string Tutorial::LoadTextFile(const std::string& file_path)
{
	std::ifstream file(file_path);
	if (!file.is_open()) {
		return "Error:ファイルがないよ～";
	}

	std::string line;
	std::string text;

	while (std::getline(file, line)) {
		text += line + "\n"; // 改行を付けてテキストを結合
	}

	file.close();
	return text;
}

//void Tutorial::LoadTextFile(const std::string& file_path)
//{
//	std::ifstream file(file_path);
//
//	std::string line;
//	int current_tutorial_num = 0;
//
//	// 一時的にテキストを構築するためのバッファ
//	std::string current_text;
//
//	while (std::getline(file, line)) {
//		// 行が番号で始まっているか判定
//		if (!line.empty() && std::isdigit(line[0])) {
//			// 前のテキストを保存
//			if (current_tutorial_num != 0) {
//				tutorial_text_list.push_back(current_text);
//				current_text.clear(); // バッファをクリア
//			}
//			current_tutorial_num = std::stoi(line.substr(0, line.find(" "))); // 数字部分を取得
//			current_text = line.substr(line.find(" ") + 1) + "\n";           // 最初の行の内容
//		}
//		else {
//			// 数字行以外はテキストに追加
//			current_text += line + "\n";
//		}
//	}
//
//	// 最後のテキストをリストに保存
//	if (current_tutorial_num != 0) {
//		tutorial_text_list.push_back(current_text);
//	}
//
//	file.close();
//
//}
void Tutorial::SetOffset()
{
	offset.x = local_location.x - 100;
	offset.y = local_location.y - 100;
	offset_size.x = local_location.x + erea.x + 100;
	offset_size.y = local_location.y;
}

bool Tutorial::GetTutorialFlg()
{
	return tutorial_flg;
}

std::string Tutorial::GetKeyName(int keyCode)
{
	if (keyMap.find(keyCode) != keyMap.end()) {
		return keyMap[keyCode];
	}
	return "不明なキー";  // マッピングされていない場合
}




