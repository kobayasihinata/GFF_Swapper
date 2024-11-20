#include "Tutorial.h"
#include "../../Scene/GameMain.h"
#include "../../Utility/DebugInfomation.h"
#include "../../Utility/PadInput.h"



// クラス内にマップを追加


Tutorial::Tutorial(): tutorial_num(1) , tutorial_time(0),tutorial_flg(false), tutorial_completed(false)
{
	in_camera = Camera::Get();
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

	tutorial_text = LoadTextFile("Resource/Dat/TutorialText.txt"); // ファイル名を指定
	//LoadTextFile("Resource/Dat/TutorialText.txt"); //

}

void Tutorial::Update(ObjectManager* _manager)
{
	SetOffset();

	// チュートリアルが進行中の場合
	if (tutorial_flg)
	{
		//tutorial_time++;
		// 指定時間経過でチュートリアル終了
		if (PadInput::OnButton(XINPUT_BUTTON_RIGHT_SHOULDER)/*tutorial_time >= 300*/)
		{
			tutorial_flg = false;
			tutorial_completed = true; // 完了フラグを設定
			tutorial_time = 0;
			tutorial_num++;
		}
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

	if (tutorial_flg)
	{
		switch (tutorial_num)
		{
		case 1:
			DrawTutorial1();
			break;
		case 2:
			//DrawTutorial2();
			break;
		case 3:
			//DrawTutorial3();
			break;
		case 4:
			//DrawTutorial4();
			break;
		default:
			break;
		}
		//DrawTutorial1();
	}

	// 現在のチュートリアル番号に対応するテキストを取得
	if (tutorial_num > 0 && tutorial_num <= tutorial_text_list.size()) {
		DrawFormatString(offset.x + 20, offset.y + 30, GetColor(255, 255, 255), tutorial_text_list[tutorial_num - 1].c_str());
	}

	DebugInfomation::Add("Tutorial Time", tutorial_time);
	DebugInfomation::Add("Tutorial NUM", tutorial_num);
	//DebugInfomation::Add("Tutorial loc", in_camera->GetCameraLocation().x);
	DebugInfomation::Add("Tutorial loc ", local_location.x);
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

	if (_object->GetObjectType() == PLAYER && !tutorial_completed)
	{
		tutorial_flg = true;
	}
}

void Tutorial::DrawTutorial1() const
{
	DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);
	// チュートリアルテキストを描画
	DrawFormatString(offset.x + 20, offset.y + 30, GetColor(255, 255, 255), tutorial_text.c_str());
}

void Tutorial::DrawTutorial2() const
{
}

void Tutorial::DrawTutorial3() const
{
}

void Tutorial::DrawTutorial4() const
{
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
	offset.x = local_location.x - 400;
	offset.y = local_location.y - 450;
	offset_size.x = local_location.x + erea.x + 400;
	offset_size.y = local_location.y + 50;
}

bool Tutorial::GetTutorialFlg()
{
	return tutorial_flg;
}



