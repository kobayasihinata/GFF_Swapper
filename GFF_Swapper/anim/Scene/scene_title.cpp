#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "scene_title.h"
#include "DxLib.h"
#include "../Utility/inputCtrl.h"
#include "../Utility/calculation.h"


//�R���X�g���N�^
Title::Title()
{
	for (int i = 0; i < 32; i++)
	{
		location[i] = { 0,0 };
		string[i] = 0;
		rela[i] = { -1,-1 };
	}
	
	inputChar = 0;
}

Title::~Title()
{
	
}

//�X�V
Scene* Title::update()
{
	//再生停止
	if (InputCtrl::GetKeyState(KEY_INPUT_SPACE) == PRESS) {
		if (flg) {
			flg = false;
		}
		else {
			flg = true;
		}
	}

	//指定したボーンから追加でボーンを生成
	if (InputCtrl::GetMouseState(R) == PRESS){
		/*int type = -1;
		int n = -1;
		for (int i = 0; i < 32; i++)
		{
			Vector2D mousePos = { (float)InputCtrl::GetMouseCursor().x,(float)InputCtrl::GetMouseCursor().y };
			if (GetVectorLength(bone[i].GetLocation(0) - mousePos) < 10) {
				type = 0;
				n = i;
				break;
			}
			if (GetVectorLength(bone[i].GetLocation(1) - mousePos) < 10) {
				type = 1;
				n = i;
				break;
			}
			
		}

		if (type != -1) {
			location[0] = bone[n].GetLocation(type);
			num = 1;
			rela[boneNum] = { n,type };
		}*/

	}



	if (InputCtrl::GetMouseState(L) == PRESS) {
		//継承
		int type = -1;
		int n = -1;
		for (int i = 0; i < 32; i++)
		{
			Vector2D mousePos = { (float)InputCtrl::GetMouseCursor().x,(float)InputCtrl::GetMouseCursor().y };
			if (GetVectorLength(bone[i].GetLocation(0) - mousePos) < 10) {
				type = 0;
				n = i;
				break;
			}
			if (GetVectorLength(bone[i].GetLocation(1) - mousePos) < 10) {
				type = 1;
				n = i;
				break;
			}

		}

		if (type != -1) {
			location[0] = bone[n].GetLocation(type);
			bone[boneNum].SetHandover(n, type);
			num = 1;
			rela[boneNum] = { n,type };
		}
		else {
			location[num] = { (float)InputCtrl::GetMouseCursor().x,(float)InputCtrl::GetMouseCursor().y };
			num++;
		}
	}

	if (num == 2) {
		/*location[1] = GetRotaLocation(location[1], location[0], angle);
		angle = 0.1f;*/
		bone[boneNum].Initialize(location[0], location[1]);
	}
	if (num == 3) {
		float cross = CrossProduct(location[1] - location[0], location[2] - location[0]); //外積　左回りの時は外積がマイナス　右回りの時はプラス
		float a = AngleOf2Vector(location[1] - location[0], location[2] - location[0]);
		if (cross < 0) {
			a = M_PI * 2 - a;
		}
		bone[boneNum].SetMoved(a, 30, 0, false);
		num = 0;
		boneNum++;
	}
	else if (InputCtrl::GetKeyState(KEY_INPUT_F) == PRESS && num == 2) {
		bone[boneNum].SetMoved(0, 0, 0, true);
		num = 0;
		boneNum++;
	}

	//update
	if (flg) {
		for (int i = 0; i < 32; i++)
		{
			if (rela[i].n != -1) {
				//中心点決め
				bone[i].SetCenterLocation(bone[rela[i].n].GetLocation(rela[i].type));
			}
			bone[i].Update();
		}
	
	}
	else {
		for (int i = 0; i < 32; i++)
		{
			bone[i].SelectUpdate();
		}
	}

	//strcpy_s(string, 32, "a");

	//文字入力
	inputChar = GetInputChar(TRUE);
	if (inputChar == 0x08 && strlen(string) > 0) { //backspace
		string[strlen(string) - 1] = '\0';
	}
	else if (inputChar != 0 && strlen(string) < 32 && inputChar != 0x08) {
		strncat(string, &inputChar, sizeof(inputChar));
	}

	return this;
}

//�`��
void Title::draw() const
{	
	DrawString(0, 0, "title", 0xffffff);

	for (int i = 0; i < num; i += 2)
	{
		if (location[i].x != 0 && location[i].y != 0) 
		{
			//DrawLine(location[i].x, location[i].y, location[i + 1].x, location[i + 1].y, 0xffffff, 1);
		}
	}

	//if (string != nullptr) {
		DrawFormatString(0, 40, 0xffffff, "%s", string);
	//}

	for (int i = 0; i < 32; i++)
	{
		bone[i].Draw();
		if (bone[i].GetSelectedFlg()) {
			bone[i].DrawUI();
		}
	}
}

