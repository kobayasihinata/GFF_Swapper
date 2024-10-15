#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "Bone.h"
#include "../Utility/inputCtrl.h"
#include "../Utility/calculation.h"

Bone::Bone()
{
	for (int i = 0; i < 32; i++)
	{
		movedAng[i] = 0.f;
		oneFrameAng[i] = 0.f;
		time[i] = 0;
		string[i] = 0;
		side[i] = true;

	}
	angle = 0.f;
	flg = false;
	num = 0;

	for (int i = 0; i < 2; i++)
	{
		BoneLoc[i] = { -1,-1 };
		BoneLocBase[i] = { -1,-1 };
	}

	centerLoc = { -1,-1 };


	selected = false;

	cursor = 0;

	handover[0] = -1;
	handover[1] = -1;

	inputChar = 0;

	fps = 0;
}

Bone::~Bone()
{
}

void Bone::Initialize(Vector2D sl, Vector2D gl)
{
	centerLoc = sl;
	BoneLoc[0] = sl - centerLoc;
	BoneLoc[1] = gl - centerLoc;

	BoneLocBase[0] = sl - centerLoc;
	BoneLocBase[1] = gl - centerLoc;
}

void Bone::Init()
{
	//flg = false;
	Vector2D worldLoc[2];
	worldLoc[0] = BoneLoc[0] + centerLoc;
	worldLoc[1] = BoneLoc[1] + centerLoc;

	Vector2D tmp = GetRotaLocation(BoneLocBase[0] + centerLoc, BoneLocBase[1] + centerLoc, movedAng[num + 1]);

	float cross = CrossProduct(worldLoc[1] - worldLoc[0], tmp - worldLoc[0]); //外積　左回りの時は外積がマイナス　右回りの時はプラス
	float a = AngleOf2Vector(worldLoc[1] - worldLoc[0], tmp - worldLoc[0]);
	if (cross < 0) {
		a = M_PI * 2 - a;
	}
	//a -= M_PI;
	//SetMoved(a, time[num + 1], num + 1, side);
	num++;
	fps = 0;
	/*BoneLoc[0] = BoneLocBase[0];
	BoneLoc[1] = BoneLocBase[1];*/

	if (time[num] == 0) {
		flg = false;
	}
}

void Bone::Update()
{
	if (flg) {
		fps++;

		if (side[num]) {//右回り
			angle += oneFrameAng[num];
			BoneLoc[1] = GetRotaLocation(BoneLocBase[1], BoneLocBase[0], angle);
			if (/*angle >= movedAng[num]*/fps >= time[num]) {
				Init();
			}
		}
		else {//左回り
			angle -= oneFrameAng[num];
			BoneLoc[1] = GetRotaLocation(BoneLocBase[1], BoneLocBase[0], angle);
			float tmpAng = angle + M_PI * 2;
			if (/*tmpAng <= movedAng[num]*/fps >= time[num]) {
				Init();
			}
		}
	}

	
}

void Bone::Draw() const
{
	//DrawLine(BoneLoc[0].x, BoneLoc[0].y, BoneLoc[1].x, BoneLoc[1].y, 0xffffff, 1);
	DrawLine(BoneLoc[0].x + centerLoc.x, BoneLoc[0].y + centerLoc.y, 
		BoneLoc[1].x + centerLoc.x, BoneLoc[1].y + centerLoc.y, 0xffffff, 1);

	for (int i = 0; i < 2; i++)
	{
		if (BoneLoc[i].x != -1) {
			DrawCircle(BoneLoc[i].x + centerLoc.x, BoneLoc[i].y + centerLoc.y, 5, 0xffffff, TRUE);
		}
	}

	if (flg) {
		for (int i = 0; time[i] != 0; i++)
		{
			Vector2D tmp;
			if (i == 0) { 
				tmp = GetRotaLocation(BoneLocBase[1], BoneLocBase[0], movedAng[i]);
			}
			else {
				float a = 0;
				for (int j = 0; j < i + 1; j++)
				{
					a += movedAng[j];
					if (a > M_PI * 2) {
						//a = M_PI * 2 - a;
						a = a - M_PI * 2;
					}
				}
				
				tmp = GetRotaLocation(BoneLocBase[1], BoneLocBase[0], a);
			}
			
			DrawCircle(tmp.x + centerLoc.x, tmp.y + centerLoc.y, 5, 0xff0000, TRUE);
		}
		
	}

	if (selected) {
		//ここでUI描画
	}


}

void Bone::SelectUpdate()
{
	Vector2D mousePos = { (float)InputCtrl::GetMouseCursor().x,(float)InputCtrl::GetMouseCursor().y };
	if (GetLineCollision(BoneLoc[0] + centerLoc, BoneLoc[1] + centerLoc, mousePos, 5) && !selected && InputCtrl::GetMouseState(R) == PRESS) {
		selected = true;
		cursor = 0;
	}

	if (selected) {
		Vector2D tmp = GetRotaLocation(BoneLocBase[1], BoneLocBase[0], movedAng[num]) + centerLoc;
		//if (GetVectorLength(tmp - mousePos) < 5) {//移動先の地点をクリックしたら
			if (InputCtrl::GetKeyState(KEY_INPUT_UP) == PRESS) {
				cursor--;
			}
			if (InputCtrl::GetKeyState(KEY_INPUT_DOWN) == PRESS) {
				cursor++;
			}

			if (flg) {
				switch (cursor)
				{
				case 0:
					if (InputCtrl::GetKeyState(KEY_INPUT_LEFT) == PRESS) {
						if (!(num == 0)) {
							num--;
						}
					}
					if (InputCtrl::GetKeyState(KEY_INPUT_RIGHT) == PRESS) {
						num++;
					}
					//cursor++;
					break;

				case 1:
					if (InputCtrl::GetKeyState(KEY_INPUT_LEFT) == PRESS) {
						side[num] = !side[num];
					}
					if (InputCtrl::GetKeyState(KEY_INPUT_RIGHT) == PRESS) {
						side[num] = !side[num];
					}
					
					//cursor++;
					break;

				case 2:
					//movedAng[num] = M_PI;

					//if (side) {//右回り
					//	oneFrameAng[num] = movedAng[num] / time[num];
					//}
					//else {//左回り
					//	oneFrameAng[num] = (M_PI * 2 - movedAng[num]) / time[num];
					//}
					//cursor++;

					if (SetMovedPosition() != -1) {
					
						
						float a = 0;
						for (int j = 0; j < num; j++)
						{
							//a = movedAng[i - 1] + movedAng[i];
							a += movedAng[j];
							if (a > M_PI * 2) {
								a = a - M_PI * 2;
							}
						}

						a = SetMovedPosition() - a;
						if (a < 0) {
							a = M_PI * 2 + a;
						}
						movedAng[num] = a;
					}
					

					break;

				case 3:
					time[num] = 180;
					//if (side) {//右回り
					//	oneFrameAng[num] = movedAng[num] / time[num];
					//}
					//else {//左回り
					//	oneFrameAng[num] = (M_PI * 2 - movedAng[num]) / time[num];
					//}
					//cursor++;




					break;

				

				case 4:
					if (InputCtrl::GetKeyState(KEY_INPUT_RETURN) == PRESS) {
						SetMoved(movedAng[num], time[num], num, side[num]);
						num = 0;
						selected = false;
					}
					break;

				default:
					break;
				}
			//}
		}
	}

}

void Bone::DrawUI() const
{
	DrawFormatString(900, 100, 0xffffff, "num : %d", num);
	DrawFormatString(900, 150, 0xffffff, "side : %d", side[num]);
	DrawFormatString(900, 200, 0xffffff, "movedAng : %f", R_D(movedAng[num]));
	DrawFormatString(900, 250, 0xffffff, "time : %d", time[num]);
	//DrawFormatString(900, 300, 0xffffff, "num : %d", num);

	DrawCircle(880, 100 + cursor * 50, 5, 0xffffff, true);

	DrawFormatString(900, 50, 0xffffff, "fps : %d", fps);
	/*for (int i = 0; i < 32; i++)
	{
		DrawFormatString(800, 50 + i * 30, 0xffffff, "side : %d", side[i]);

	}*/

}

float Bone::SetMovedPosition()
{
	Vector2D mousePos;
	if (InputCtrl::GetMouseState(R) == PRESS) {
		mousePos = { (float)InputCtrl::GetMouseCursor().x,(float)InputCtrl::GetMouseCursor().y };

		Vector2D worldLoc[2];
		worldLoc[0] = BoneLoc[0] + centerLoc;
		worldLoc[1] = BoneLoc[1] + centerLoc;

		//worldLoc[1] = GetRotaLocation(BoneLoc[1], BoneLoc[0], movedAng[num - 1]) + centerLoc;
		//GetRotaLocation(BoneLocBase[1], BoneLocBase[0], angle);

		float cross = CrossProduct(worldLoc[1] - worldLoc[0], mousePos - worldLoc[0]); //外積　左回りの時は外積がマイナス　右回りの時はプラス
		float a = AngleOf2Vector(worldLoc[1] - worldLoc[0], mousePos - worldLoc[0]);
		if (cross < 0) {
			a = M_PI * 2 - a;
		}

		return a;
	}

	return -1;
}

void Bone::SetMoved(float ang, int t, int n, bool f)
{
	movedAng[n] = ang;
	time[n] = t;
	if (f) {//右回り
		oneFrameAng[n] = ang / t;
	}
	else {//左回り
		oneFrameAng[n] = (M_PI * 2 - ang) / t;
	}
	if (ang == 0) {
		oneFrameAng[n] = 0;
	}
	side[num] = f;
	//num = n;
	flg = true;
}

void Bone::InputString()
{
	inputChar = GetInputChar(TRUE);
	if (inputChar == 0x08 && strlen(string) > 0) { //backspace
		string[strlen(string) - 1] = '\0';
	}
	else if (inputChar != 0 && strlen(string) < 32 && inputChar != 0x08) {
		strncat(string, &inputChar, sizeof(inputChar));
	}
}
