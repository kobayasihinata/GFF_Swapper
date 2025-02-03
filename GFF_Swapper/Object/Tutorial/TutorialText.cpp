#include "DxLib.h"
#include "TutorialText.h"
#include "../../Utility/DebugInformation.h"


TutorialText::TutorialText() : GNum(0), GColor(GREEN), Gbutton_draw{ false, false, false, false }, GGNum(0), frame(0), Gstick_angle(0.0), stage_height(0), p_c(GREEN), add_x(0), add_y(0), f_c(RED), Gdraw_stick_shift{ 0, 0 }, circleAng(0.f), cosor_flg(false), p_swap_x(0), e_swap_x(0)
{
	in_camera = Camera::Get();
	firenum = 0;
	reflection = false;
}

TutorialText::~TutorialText()
{
}

void TutorialText::Update(Vector2D _p, int y)
{

	frame++;
	stage_height = y;

	if (frame % 10 == 0)
	{
		Gbutton_draw[0] = !Gbutton_draw[0];
	}
	// チュートリアルエリア：左スティック
	Gstick_angle += 0.05f;
	if (Gstick_angle > 1)
	{
		Gstick_angle = 0;
	}

	Gdraw_stick_shift.x = cosf(Gstick_angle * M_PI * 2) * 5;
	Gdraw_stick_shift.y = sinf(Gstick_angle * M_PI * 2) * 5;

	// 最初の緑ブロックのところ
	if (_p.x >= 1540.0f && _p.y <= 2680.0f && _p.x <= 2600.f && _p.y >= 2000.f)
	{
		if (GColor == GREEN && GNum != 26 || GColor == BLUE && GNum < 160)
		{
			GNum += 2;
		}
		else
		{
			GNum;
			GGNum++;
			if (GGNum > 60)
			{
				if (GColor == GREEN)
				{
					GColor = BLUE;
				}
				else
				{
					GColor = GREEN;
				}

				GGNum = 0;
				GNum = 0;
			}

		}
	}

	if (_p.x >= 3780.f && _p.y <= 2580.f && _p.x <= 5000.f && _p.y >= 2000.f)
	{
		Gbutton_draw[1] = true;

		if (cosor_flg == false)
		{
			cosor_flg = true;
		}
		p_c = WHITE;

		if (p_swap_x < 230)
		{
			p_swap_x += 6;
		}
		else
		{
			f_c = GREEN;
			if (e_swap_x > -240)
			{
				e_swap_x -= 6;
			}
			else
			{
				p_c = RED;
			}
		}
		
	}
	else
	{
		Gbutton_draw[1] = false;
		p_c = GREEN;
		f_c = RED;
		p_swap_x = 0;
		e_swap_x = 0;
	}

	// >= 5400 >=2580 青い蝙蝠の前
	if (_p.x >= 5400.f && _p.y <= 2680.f && _p.x <= 6800.f && _p.y >= 2000.f)
	{
		if (add_x < 220)
		{
			add_x += 3;
		}
		else
		{
			if (Gbutton_draw[3] == false)
			{
				Gbutton_draw[3] = true;
			}
			else if (Gbutton_draw[3] == true)
			{
				Gbutton_draw[3] = false;
			}

			add_x = 0;
		}
	}

	/*if (reflection == false)
	{
		if (firenum < 390)
		{
			firenum += 5;
		}
		else
		{
			reflection = true;
		}
	}
	else
	{
		if (reflection == true)
		{
			if (firenum)
			{
				firenum -= 5;
			}
			else
			{
				reflection = false;
			}
		}
	}*/

	if (frame > 59)
	{
		frame = 0;
	}

	if (circleAng++ >= 360.f) {
		circleAng = 0.f;
	}
}

void TutorialText::Draw() const
{
	//DebugInformation::Add("Tutorialtext loc", in_camera->GetCameraLocation().x);
	DebugInformation::Add("Tutorialtext height", stage_height);
	SetFontSize(50);

	//// 最初のチュートリアルテキストボックス
	//DrawBoxAA(515 - in_camera->GetCameraLocation().x, stage_height - 546 - in_camera->GetCameraLocation().y, 903 - in_camera->GetCameraLocation().x, stage_height - 317 - in_camera->GetCameraLocation().y, 0xffffff, FALSE, 3.0f);
	//DrawBoxAA(518 - in_camera->GetCameraLocation().x, stage_height - 543 - in_camera->GetCameraLocation().y, 900 - in_camera->GetCameraLocation().x, stage_height - 320 - in_camera->GetCameraLocation().y, 0x555555, TRUE, 3.0f);

	//// 左スティック：移動の説明
	//DrawCircleAA(580 - in_camera->GetCameraLocation().x, stage_height - 480 - in_camera->GetCameraLocation().y, 25, 100, 0x000000, TRUE);
	//DrawCircleAA(580 - in_camera->GetCameraLocation().x, stage_height - 480 - in_camera->GetCameraLocation().y, 25, 100, 0x666666, FALSE, 3.0f);
	//DrawCircleAA(580 - in_camera->GetCameraLocation().x + Gdraw_stick_shift.x, stage_height - 480 - in_camera->GetCameraLocation().y + Gdraw_stick_shift.y, 18, 100, 0x666666, TRUE);
	//DrawStringF(610 - in_camera->GetCameraLocation().x, stage_height - 500 - in_camera->GetCameraLocation().y, "：Move", 0xffffff);

	//if (Gbutton_draw[0] == false)
	//{
	//	// A ボタンイメージ描画
	//	DrawCircleAA(577 - in_camera->GetCameraLocation().x, stage_height - 460 - in_camera->GetCameraLocation().y + 60, 25, 100, 0x557b0e, FALSE);
	//	DrawStringF(577 - in_camera->GetCameraLocation().x - 12, stage_height - 460 - in_camera->GetCameraLocation().y + 38, "A", 0x557b0e);
	//}
	//else
	//{
	//	DrawCircleAA(577 - in_camera->GetCameraLocation().x, stage_height - 460 - in_camera->GetCameraLocation().y + 55, 25, 100, 0x557b0e, TRUE);
	//	DrawCircleAA(577 - in_camera->GetCameraLocation().x, stage_height - 460 - in_camera->GetCameraLocation().y + 60, 25, 100, 0x557b0e, TRUE);
	//	DrawStringF(577 - in_camera->GetCameraLocation().x - 12, stage_height - 460 - in_camera->GetCameraLocation().y + 33, "A", 0x000000);
	//}
	//DrawStringF(610 - in_camera->GetCameraLocation().x, stage_height - 428 - in_camera->GetCameraLocation().y, "：Jump", 0xffffff);

	//// 色ブロックに乗れるかチュートリアルテキスト
	//DrawBoxAA(1742 - in_camera->GetCameraLocation().x, stage_height - 700 - in_camera->GetCameraLocation().y, 2073 - in_camera->GetCameraLocation().x, stage_height - 397 - in_camera->GetCameraLocation().y, 0xffffff, FALSE, 3.0f);
	//DrawBoxAA(1745 - in_camera->GetCameraLocation().x, stage_height - 697 - in_camera->GetCameraLocation().y, 2070 - in_camera->GetCameraLocation().x, stage_height - 400 - in_camera->GetCameraLocation().y, 0x555555, TRUE, 3.0f);

	//// 緑ブロック描画
	//if (GColor == GREEN)
	//{
	//	// 赤　マル
	//	DrawCircleAA(1960 - in_camera->GetCameraLocation().x, stage_height - 630 - in_camera->GetCameraLocation().y, 40, 100, 0xCC3300, FALSE, 10.0f);
	//	for (int i = 0; i < 160; i += 40)
	//	{
	//		DrawBoxAA(1800 - in_camera->GetCameraLocation().x + i, stage_height - 540 - in_camera->GetCameraLocation().y, 1840 - in_camera->GetCameraLocation().x + i, stage_height - 500 - in_camera->GetCameraLocation().y, GColor, TRUE);
	//		DrawBoxAA(1810 - in_camera->GetCameraLocation().x + i, stage_height - 540 - in_camera->GetCameraLocation().y + 20, 1815 - in_camera->GetCameraLocation().x + i, stage_height - 540 - in_camera->GetCameraLocation().y + 25, 0x999999, true);
	//		DrawBoxAA(1830 - in_camera->GetCameraLocation().x + i, stage_height - 540 - in_camera->GetCameraLocation().y + 15, 1835 - in_camera->GetCameraLocation().x + i, stage_height - 540 - in_camera->GetCameraLocation().y + 20, 0x999999, true);
	//		DrawBoxAA(1825 - in_camera->GetCameraLocation().x + i, stage_height - 540 - in_camera->GetCameraLocation().y + 35, 1830 - in_camera->GetCameraLocation().x + i, stage_height - 540 - in_camera->GetCameraLocation().y + 40, 0x999999, true);
	//	}
	//}
	//else
	//{
	//	// 青 バツ
	//	DrawLineAA(1910 - in_camera->GetCameraLocation().x, stage_height - 680 - in_camera->GetCameraLocation().y, 2012 - in_camera->GetCameraLocation().x, stage_height - 580 - in_camera->GetCameraLocation().y, 0x3300CC, 20.0f);
	//	DrawLineAA(2012 - in_camera->GetCameraLocation().x, stage_height - 680 - in_camera->GetCameraLocation().y, 1910 - in_camera->GetCameraLocation().x, stage_height - 580 - in_camera->GetCameraLocation().y, 0x3300CC, 20.0f);

	//	// 青ブロック描画
	//	for (int i = 0; i < 160; i += 40)
	//	{
	//		DrawBoxAA(1800 - in_camera->GetCameraLocation().x + i, stage_height - 540 - in_camera->GetCameraLocation().y, 1840 - in_camera->GetCameraLocation().x + i, stage_height - 500 - in_camera->GetCameraLocation().y, GColor, TRUE);
	//	}
	//}
	//// チュートリアルテキスト プレイヤー描画
	//GDrawPlayer(0, 0, 0, GNum, GREEN);

	//// 色交換のチュートリアルテキスト
	//DrawBoxAA(4100 - in_camera->GetCameraLocation().x, stage_height - 650 - in_camera->GetCameraLocation().y, 4503 - in_camera->GetCameraLocation().x, stage_height - 400 - in_camera->GetCameraLocation().y, 0xffffff, FALSE, 3.0f);
	//DrawBoxAA(4103 - in_camera->GetCameraLocation().x, stage_height - 647 - in_camera->GetCameraLocation().y, 4500 - in_camera->GetCameraLocation().x, stage_height - 403 - in_camera->GetCameraLocation().y, 0x555555, TRUE, 3.0f);

	//if (Gbutton_draw[1] == false)
	//{
	//	// B ボタンイメージ描画
	//	DrawCircleAA(577 - in_camera->GetCameraLocation().x + 3600, stage_height - 460 - in_camera->GetCameraLocation().y + 55 - 200, 25, 100, 0xc5482c, TRUE);
	//	DrawCircleAA(577 - in_camera->GetCameraLocation().x + 3600, stage_height - 460 - in_camera->GetCameraLocation().y + 60 - 200, 25, 100, 0xc5482c, TRUE);
	//	DrawStringF(577 - in_camera->GetCameraLocation().x - 12 + 3600, stage_height - 460 - in_camera->GetCameraLocation().y + 33 - 200, "B", 0x000000);
	//}
	//else
	//{
	//	DrawCircleAA(577 - in_camera->GetCameraLocation().x + 3600, stage_height - 460 - in_camera->GetCameraLocation().y + 60 - 200, 25, 100, 0xc5482c, FALSE);
	//	DrawStringF(577 - in_camera->GetCameraLocation().x - 12 + 3600, stage_height - 460 - in_camera->GetCameraLocation().y + 38 - 200, "B", 0xc5482c);
	//}

	//if (Gbutton_draw[1] == true)
	//{
	//	//選択カーソル
	//	GDrawCircle(cosor_flg);
	//}

	//DrawStringF(4210 - in_camera->GetCameraLocation().x, stage_height - 623 - in_camera->GetCameraLocation().y, "＆", 0xffffff);

	//// 左スティック描画
	//DrawCircleAA(580 - in_camera->GetCameraLocation().x + 3710, stage_height - 480 - in_camera->GetCameraLocation().y - 121, 25, 100, 0x000000, TRUE);
	//DrawCircleAA(580 - in_camera->GetCameraLocation().x + 3710, stage_height - 480 - in_camera->GetCameraLocation().y - 121, 25, 100, 0x666666, FALSE, 3.0f);
	//DrawCircleAA(580 - in_camera->GetCameraLocation().x + Gdraw_stick_shift.x + 3710, stage_height - 480 - in_camera->GetCameraLocation().y + Gdraw_stick_shift.y - 121, 18, 100, 0x666666, TRUE);

	//// チュートリアルテキスト プレイヤー描画
	//GDrawPlayer(2350, -130,0, 0, p_c);
	//// チュートリアルテキスト 蛙描画
	//GDrawFrag();

	//// 赤ブロック
	//DrawBoxAA(1800 - in_camera->GetCameraLocation().x + 2600, stage_height - 540 - in_camera->GetCameraLocation().y + 65, 1840 - in_camera->GetCameraLocation().x + 2600, stage_height - 500 - in_camera->GetCameraLocation().y + 65, RED, TRUE);
	//
	//if (Gbutton_draw[1] == true && cosor_flg == true)
	//{
	//	if (p_swap_x < 230)
	//	{
	//		// player 緑
	//		DrawBoxAA((4170 + p_swap_x) - in_camera->GetCameraLocation().x, stage_height - 540 - in_camera->GetCameraLocation().y, (4190 + p_swap_x) - in_camera->GetCameraLocation().x, stage_height - 520 - in_camera->GetCameraLocation().y, GREEN, TRUE);
	//		DrawBoxAA((4170 + p_swap_x) - in_camera->GetCameraLocation().x, stage_height - 540 - in_camera->GetCameraLocation().y, (4190 + p_swap_x) - in_camera->GetCameraLocation().x, stage_height - 520 - in_camera->GetCameraLocation().y, 0x000000, FALSE);
	//	}

	//	if (e_swap_x > -240 && p_swap_x > 230)
	//	{
	//		// frog 赤
	//		DrawBoxAA((4410 - -e_swap_x) - in_camera->GetCameraLocation().x, stage_height - 540 - in_camera->GetCameraLocation().y, (4430 - -e_swap_x) - in_camera->GetCameraLocation().x, stage_height - 520 - in_camera->GetCameraLocation().y, RED, TRUE);
	//		DrawBoxAA((4410 - -e_swap_x) - in_camera->GetCameraLocation().x, stage_height - 540 - in_camera->GetCameraLocation().y, (4430 - -e_swap_x) - in_camera->GetCameraLocation().x, stage_height - 520 - in_camera->GetCameraLocation().y, 0x000000, FALSE);
	//	}
	//}
	//else if(Gbutton_draw[1] == true && cosor_flg == false)
	//{

	//}

	//// ダメージゾーンのチュートリアルテキスト
	//DrawBoxAA(5950 - in_camera->GetCameraLocation().x, stage_height - 500 - in_camera->GetCameraLocation().y, 6243 - in_camera->GetCameraLocation().x, stage_height - 260 - in_camera->GetCameraLocation().y, 0xffffff, FALSE, 3.0f);
	//DrawBoxAA(5953 - in_camera->GetCameraLocation().x, stage_height - 497 - in_camera->GetCameraLocation().y, 6240 - in_camera->GetCameraLocation().x, stage_height - 263 - in_camera->GetCameraLocation().y, 0x555555, TRUE, 3.0f);

	//// 水描画
	//for (int i = 0; i < 6; i++)
	//{
	//	for (int j = 0; j < 2; j++)
	//	{
	//		ResourceManager::StageAnimDraw({ (6040 + (j * BOX_WIDTH)) - in_camera->GetCameraLocation().x, stage_height - (500 - (i * BOX_HEIGHT)) - in_camera->GetCameraLocation().y }, 5);
	//	}
	//}

	//if (Gbutton_draw[3] == false)
	//{
	//	GDrawPlayer(4150, -300, add_x, 0, RED);
	//	// 青 バツ
	//	DrawLineAA(5960 - in_camera->GetCameraLocation().x, stage_height - 480 - in_camera->GetCameraLocation().y, 6030 - in_camera->GetCameraLocation().x, stage_height - 410 - in_camera->GetCameraLocation().y, 0x3300CC, 20.0f);
	//	DrawLineAA(5960 - in_camera->GetCameraLocation().x, stage_height - 410 - in_camera->GetCameraLocation().y, 6030 - in_camera->GetCameraLocation().x, stage_height - 480 - in_camera->GetCameraLocation().y, 0x3300CC, 20.0f);
	//	SetFontSize(35);
	//	DrawString(6122 - in_camera->GetCameraLocation().x, stage_height - 460 - in_camera->GetCameraLocation().y, "Damage", 0xff0000);
	//}
	//else
	//{
	//	GDrawPlayer(4150, -300, add_x, 0, BLUE);
	//	// 赤　マル
	//	DrawCircleAA(6000 - in_camera->GetCameraLocation().x, stage_height - 450 - in_camera->GetCameraLocation().y, 30, 100, 0xCC3300, FALSE, 10.0f);
	//}
	
	// ボスエリアのチュートリアルテキスト
	//DrawBoxAA(15415 - in_camera->GetCameraLocation().x, stage_height - 680 - in_camera->GetCameraLocation().y, 16003 - in_camera->GetCameraLocation().x, stage_height - 360 - in_camera->GetCameraLocation().y, 0xffffff, FALSE, 3.0f);
	//DrawBoxAA(15418 - in_camera->GetCameraLocation().x, stage_height - 677 - in_camera->GetCameraLocation().y, 16000 - in_camera->GetCameraLocation().x, stage_height - 363 - in_camera->GetCameraLocation().y, 0x555555, TRUE, 3.0f);

	//SetFontSize(150);
	//// ボス(黒丸)
	//DrawCircleAA(15830 - in_camera->GetCameraLocation().x, stage_height - 550 - in_camera->GetCameraLocation().y, 100, 100, 0xff0000, TRUE);
	//DrawCircleAA(15830 - in_camera->GetCameraLocation().x, stage_height - 550 - in_camera->GetCameraLocation().y, 100, 100, 0xffffff, FALSE, 2.0f);
	//DrawStringF(15755 - in_camera->GetCameraLocation().x, stage_height - 620 - in_camera->GetCameraLocation().y, "？", 0xffffff);

	//// 火球
	//ResourceManager::StageAnimDraw({ 15830 - in_camera->GetCameraLocation().x - 20 - firenum,stage_height - 550 - in_camera->GetCameraLocation().y - 20 }, FIRE);
	//DrawCircleAA(15830 - in_camera->GetCameraLocation().x - firenum, stage_height - 550 - in_camera->GetCameraLocation().y, 20, 100, 0xff0000, TRUE);
	//ResourceManager::DrawRotaBox(15830 - in_camera->GetCameraLocation().x - firenum, stage_height - 550 - in_camera->GetCameraLocation().y, 20, 20, 15830 - in_camera->GetCameraLocation().x - firenum, stage_height - 550 - in_camera->GetCameraLocation().y, 45, 0xff0000, TRUE);
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120 + (frame % 75));
	//DrawCircleAA(15830 - in_camera->GetCameraLocation().x - firenum, stage_height - 550 - in_camera->GetCameraLocation().y, 20, 100, 0x000000, TRUE);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);


}

void TutorialText::GDrawPlayer(int xNum, int yNum, int add_x, int add_y, int _c)const
{
	//頭
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 646 + yNum - in_camera->GetCameraLocation().y - add_y, 23, 15, 1830 - in_camera->GetCameraLocation().x, stage_height - 646 - in_camera->GetCameraLocation().y, 0, _c, true);
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 646 + yNum - in_camera->GetCameraLocation().y - add_y, 23, 15, 1830 - in_camera->GetCameraLocation().x, stage_height - 646 - in_camera->GetCameraLocation().y, 0, 0x000000, false);

	//目
	ResourceManager::DrawRotaBox(1824 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 646 + yNum - in_camera->GetCameraLocation().y - add_y, 6, 7, 1830 - in_camera->GetCameraLocation().x, stage_height - 646 - in_camera->GetCameraLocation().y, 0, 0x000000, true);

	//首
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 660 + yNum - in_camera->GetCameraLocation().y - add_y, 10, 5, 1830 - in_camera->GetCameraLocation().x, stage_height - 646 - in_camera->GetCameraLocation().y, 0, _c, true);
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 660 + yNum - in_camera->GetCameraLocation().y - add_y, 10, 5, 1830 - in_camera->GetCameraLocation().x, stage_height - 646 - in_camera->GetCameraLocation().y, 0, 0x000000, false);

	//胴体																					
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 686 + yNum - in_camera->GetCameraLocation().y - add_y, 21, 37, 1830 - in_camera->GetCameraLocation().x, stage_height - 646 - in_camera->GetCameraLocation().y, 0, _c, true);
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 686 + yNum - in_camera->GetCameraLocation().y - add_y, 21, 37, 1830 - in_camera->GetCameraLocation().x, stage_height - 646 - in_camera->GetCameraLocation().y, 0, 0x000000, false);

	//バッグ
	ResourceManager::DrawRotaBox(1845 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 686 + yNum - in_camera->GetCameraLocation().y - add_y, 5, 23, 1830 - in_camera->GetCameraLocation().x, stage_height - 646 - in_camera->GetCameraLocation().y, 0, _c, true);
	ResourceManager::DrawRotaBox(1845 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 686 + yNum - in_camera->GetCameraLocation().y - add_y, 5, 23, 1830 - in_camera->GetCameraLocation().x, stage_height - 646 - in_camera->GetCameraLocation().y, 0, 0x000000, false);
	ResourceManager::DrawRotaBox(1845 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 686 + yNum - in_camera->GetCameraLocation().y - add_y, 3, 15, 1830 - in_camera->GetCameraLocation().x, stage_height - 646 - in_camera->GetCameraLocation().y, 0, 0x000000, true);

	//腕
	ResourceManager::DrawRotaBox(1815 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 616 + yNum - in_camera->GetCameraLocation().y - add_y, 28, 7, 1830 - in_camera->GetCameraLocation().x, stage_height - 616 - in_camera->GetCameraLocation().y, 0, _c, true);
	ResourceManager::DrawRotaBox(1815 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 616 + yNum - in_camera->GetCameraLocation().y - add_y, 28, 7, 1830 - in_camera->GetCameraLocation().x, stage_height - 616 - in_camera->GetCameraLocation().y, 0, 0x000000, false);

	//足												 														
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 651 + yNum - in_camera->GetCameraLocation().y - add_y, 7, 27, 1830 - in_camera->GetCameraLocation().x, stage_height - 616 - in_camera->GetCameraLocation().y, 0, _c, true);
	ResourceManager::DrawRotaBox(1830 - xNum - in_camera->GetCameraLocation().x - add_x, stage_height - 651 + yNum - in_camera->GetCameraLocation().y - add_y, 7, 27, 1830 - in_camera->GetCameraLocation().x, stage_height - 616 - in_camera->GetCameraLocation().y, 0, 0x000000, false);

	//帽子　中央
	DrawTriangleAA(1830 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 667 - yNum - in_camera->GetCameraLocation().y + add_y, 1820 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 651 - yNum - in_camera->GetCameraLocation().y + add_y, 1840 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 651 - yNum - in_camera->GetCameraLocation().y + add_y, _c, true);
	DrawTriangleAA(1830 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 667 - yNum - in_camera->GetCameraLocation().y + add_y, 1820 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 651 - yNum - in_camera->GetCameraLocation().y + add_y, 1840 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 651 - yNum - in_camera->GetCameraLocation().y + add_y, 0x000000, false);
	//帽子　右側
	DrawTriangleAA(1830 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 667 - yNum - in_camera->GetCameraLocation().y + add_y, 1840 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 651 - yNum - in_camera->GetCameraLocation().y + add_y, 1850 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 655 - yNum - in_camera->GetCameraLocation().y + add_y, _c, true);
	DrawTriangleAA(1830 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 667 - yNum - in_camera->GetCameraLocation().y + add_y, 1840 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 651 - yNum - in_camera->GetCameraLocation().y + add_y, 1850 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 655 - yNum - in_camera->GetCameraLocation().y + add_y, 0x000000, false);
	//帽子　左側
	DrawTriangleAA(1830 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 667 - yNum - in_camera->GetCameraLocation().y + add_y, 1810 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 655 - yNum - in_camera->GetCameraLocation().y + add_y, 1820 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 651 - yNum - in_camera->GetCameraLocation().y + add_y, _c, true);
	DrawTriangleAA(1830 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 667 - yNum - in_camera->GetCameraLocation().y + add_y, 1810 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 655 - yNum - in_camera->GetCameraLocation().y + add_y, 1820 + xNum - in_camera->GetCameraLocation().x + add_x, stage_height - 651 - yNum - in_camera->GetCameraLocation().y + add_y, 0x000000, false);
}

void TutorialText::GDrawFrag()const
{
	//胴体
	ResourceManager::DrawRotaBox(4410 - in_camera->GetCameraLocation().x, stage_height - 530 - in_camera->GetCameraLocation().y, 50, 25,4410 - in_camera->GetCameraLocation().x, stage_height - 530 - in_camera->GetCameraLocation().y, 0, f_c, TRUE);
	ResourceManager::DrawRotaBox(4410 - in_camera->GetCameraLocation().x, stage_height - 530 - in_camera->GetCameraLocation().y, 50, 25,4410 - in_camera->GetCameraLocation().x, stage_height - 530 - in_camera->GetCameraLocation().y, 0, 0x000000, FALSE);

	//付け根側後ろ足
	ResourceManager::DrawRotaBox(4423 - in_camera->GetCameraLocation().x, stage_height - 520 - in_camera->GetCameraLocation().y, 30, 10, 4423 - in_camera->GetCameraLocation().x, stage_height - 520 - in_camera->GetCameraLocation().y, 0, f_c, TRUE);
	ResourceManager::DrawRotaBox(4423 - in_camera->GetCameraLocation().x, stage_height - 520 - in_camera->GetCameraLocation().y, 30, 10, 4423 - in_camera->GetCameraLocation().x, stage_height - 520 - in_camera->GetCameraLocation().y, 0, 0x000000, FALSE);
													  																 
	//後ろ足先端																									 
	ResourceManager::DrawRotaBox(4424 - in_camera->GetCameraLocation().x, stage_height - 510 - in_camera->GetCameraLocation().y, 40, 10, 4424 - in_camera->GetCameraLocation().x, stage_height - 510 - in_camera->GetCameraLocation().y, 0, f_c, TRUE);
	ResourceManager::DrawRotaBox(4424 - in_camera->GetCameraLocation().x, stage_height - 510 - in_camera->GetCameraLocation().y, 40, 10, 4424 - in_camera->GetCameraLocation().x, stage_height - 510 - in_camera->GetCameraLocation().y, 0, 0x000000, FALSE);
	
	//目
	ResourceManager::DrawRotaBox(4382 - in_camera->GetCameraLocation().x, stage_height - 538 - in_camera->GetCameraLocation().y, 10, 10, 4382 - in_camera->GetCameraLocation().x, stage_height - 538 - in_camera->GetCameraLocation().y, 0, 0xffffff, TRUE);
	ResourceManager::DrawRotaBox(4382 - in_camera->GetCameraLocation().x, stage_height - 538 - in_camera->GetCameraLocation().y, 10, 10, 4382 - in_camera->GetCameraLocation().x, stage_height - 538 - in_camera->GetCameraLocation().y, 0, 0x000000, FALSE);
}

void TutorialText::GDrawCircle(bool f) const
{
	Vector2D base;
	if (f) {
		DrawCircleAA(4410 - in_camera->GetCameraLocation().x, stage_height - 530 - in_camera->GetCameraLocation().y, 40, 40, 0xffff00, FALSE, 4);
		base.x = 4410 - in_camera->GetCameraLocation().x;
		base.y = stage_height - 530 - in_camera->GetCameraLocation().y;
	}
	else {
		DrawCircleAA(1800 - in_camera->GetCameraLocation().x + 2600 + 20, stage_height - 540 - in_camera->GetCameraLocation().y + 65 + 20, 40, 40, 0xffff00, FALSE, 4);
		base.x = 1800 - in_camera->GetCameraLocation().x + 2600 + 20;
		base.y = stage_height - 540 - in_camera->GetCameraLocation().y + 65 + 20;
	}
	Vector2D l[3];
	l[0].x = base.x;
	l[0].y = base.y - 40;

	l[0] = RotationLocation(base, l[0], (float)(circleAng * M_PI / 180));
	l[1] = RotationLocation(base, l[0], (float)(120.f * M_PI / 180));
	l[2] = RotationLocation(base, l[0], (float)(240.f * M_PI / 180));

	DrawCircleAA(l[0].x, l[0].y, 15, 32, 0xcc0000, TRUE);
	DrawCircleAA(l[1].x, l[1].y, 15, 32, 0x3c78d8, TRUE);
	DrawCircleAA(l[2].x, l[2].y, 15, 32, 0x6aa84f, TRUE);
}

Vector2D TutorialText::RotationLocation(Vector2D BaseLoc, Vector2D Loc, float r) const
{
	Vector2D l;
	l.x = Loc.x - BaseLoc.x;
	l.y = Loc.y - BaseLoc.y;

	Vector2D ret;
	ret.x = l.x * cosf(r) - l.y * sinf(r);
	ret.y = l.x * sinf(r) + l.y * cosf(r);

	ret.x += BaseLoc.x;
	ret.y += BaseLoc.y;

	return ret;
}
