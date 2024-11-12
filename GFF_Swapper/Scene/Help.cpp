#include "Help.h"
#include "DxLib.h"
#include "Title.h"
#include "GameMain.h"
#include "../Utility/PadInput.h"
#include "../Utility/KeyInput.h"
#include "../Utility/ResourceManager.h"

Help::Help() :l_help_red{ 360.f, 120.f }, l_help_blue{ 420.f, 120.f }, l_help_fire{ 360, 330 }, l_help_water{ 420, 330 }, l_help_wood{ 480, 330 }, l_help_bamboo{ 540, 330 }, l_help_respawn{ 450, 600 }, player_location{ 430, 210 }, p_color(RED), p_hp(5), sign{ '<', '=' },
l_Deer{ 820, 140 }, l_Frog{ 960, 165 }, l_Bat{ 1060, 140 }, cursor_se(0), decision_se(0), DamageColor(RED), Gameoverflg(true), g_cnt(0)
{

}

Help::~Help()
{

}

void Help::Initialize()
{
	cursor_se = ResourceManager::SetSound("Resource/Sounds/Player/cursor.wav");
	decision_se = ResourceManager::SetSound("Resource/Sounds/System/decision.wav");
	ResourceManager::SetSoundVolume(decision_se, 200);
}

void Help::Finalize()
{

}

AbstractScene* Help::Update()
{
	if (++frame > 6000)
	{
		frame = 0;
	}

	switch (MenuNumber)
	{
	case 1:
		p_hp = 5;
		DamageColor = RED;
		p_color = RED;

		break;

	case 2:
		l_help_red = { 965.f, 340.f };
		l_help_blue = { 965.f, 340.f };

		l_help_fire = { 965.f, 475.f };
		l_help_water = { 965.f, 475.f };
		l_help_wood = { 905.f, 475.f };
		l_help_bamboo = { 1045.f, 475.f };

		if (frame % 60 == 0 && p_hp > 0)
		{
			p_hp--;

			switch (DamageColor)
			{
			case RED:
				DamageColor = BLUE;
				p_color = RED;
				break;

			case BLUE:
				DamageColor = GREEN;
				p_color = BLUE;
				break;

			case GREEN:
				DamageColor = RED;
				if (p_hp < 0)
				{
					p_color = RED;
				}
				else
				{
					p_color = GREEN;
				}
				
				break;

			default:
				break;
			}
		}
		else
		{
			if ( p_hp <= 0 && frame % 10 == 0)
			{
				Gameoverflg = !Gameoverflg;
				g_cnt++;
			}
		}

		break;

	case 3:
		p_hp = 5;
		DamageColor = RED;
		p_color = RED;

		l_help_red = { 360.f, 120.f };
		l_help_blue = { 420.f, 120.f };

		l_help_fire = { 360, 330 };
		l_help_water = { 420, 330 };
		l_help_wood = { 480, 330 };
		l_help_bamboo = { 540, 330 };

		break;

	case 4:
		p_hp = 5;
		DamageColor = RED;
		p_color = RED;

		break;

	case 5:
		p_hp = 5;
		DamageColor = RED;
		p_color = RED;

		if (frame % 10 == 0)
		{
			button_draw = !button_draw;
		}
		break;

	default:
		break;
	}

	if (wt < 15)
	{
		wt++;
	}
		
	// 下がる
	if (PadInput::TipLStick(STICKL_Y) < -0.8f && wt >= 15 || PadInput::OnButton(XINPUT_BUTTON_DPAD_DOWN))
	{
		ResourceManager::StartSound(cursor_se);
		if (++MenuNumber < 6) {}
		if (MenuNumber > 4)MenuNumber = 5;

		wt = 0;
	}
	// 上がる
	else if (PadInput::TipLStick(STICKL_Y) > 0.8f && wt >= 15 || PadInput::OnButton(XINPUT_BUTTON_DPAD_UP))
	{
		ResourceManager::StartSound(cursor_se);
		if (--MenuNumber > 5){}
		if (MenuNumber < 1) MenuNumber = 1;

		wt = 0;
	}

	// 操作方法の説明
	if (PadInput::OnButton(XINPUT_BUTTON_B) == 1)
	{
		ResourceManager::StartSound(decision_se);
		switch (MenuNumber)
		{
		case 5:
			return new Title();
			break;
		default:
			break;
		}
	}

	if (p_hp <= 0 && g_cnt > 10)
	{
		p_hp = 5;
		g_cnt = 0;
	}

	return this;
}

void Help::Draw()const
{
	DrawBoxAA(320.f, 20.f, 1260.f, 700.f, 0xffffff, FALSE, 7.0f);
	
	//大外側
	DrawBoxAA(10.f, 20.f, 300.f, 130.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 160.f, 300.f, 270.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 300.f, 300.f, 410.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 440.f, 300.f, 560.f, 0xeeeeee, FALSE, 7.0f);
	DrawBoxAA(10.f, 590.f, 300.f, 700.f, 0xeeeeee, FALSE, 7.0f);

	SetFontSize(49); //デフォルトでは 
	// 多分漢字の一文字の長さは 25
	DrawString(40, 50, "UserGuide", 0xffffff);	//操作方法
	DrawString(80, 190, "Player", 0xffffff);		//Playerについて
	DrawString(100, 330, "Stage", 0xffffff);		//ステージにある色ブロックについて(色ブロックとかダメージゾーンとか)
	DrawString(40, 475, "TypeChart", 0xffffff);	//属性表
	DrawString(70, 620, " Title", 0xffffff);		//タイトルへ

	switch (MenuNumber)
	{
		
	case 1:	//操作方法

		//カーソル描画
		DrawLineAA(30.f, 100.f, 280.f, 100.f, 0xff0000, 5.0f);

		//左スティック：移動
		DrawCircleAA(385.f, 77.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(385.f, 77.f, 30.f, 32, 0x525156);
		DrawString(470, 50, "LeftStick : Move", 0xffffff);	

		//Aボタン：ジャンプ
		DrawCircleAA(385.f, 187.f, 50.f,  32, 0x1c2b3e);
		DrawCircleAA(385.f, 187.f, 30.f, 32, 0x557b0e);
		DrawString(373, 160, "A", 0xffffff);
		DrawString(470, 164, "A Button : Jump", 0xffffff);

		//Bボタン\n(長押し)：色の交換
		DrawCircleAA(385.f, 295.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(385.f, 295.f, 30.f, 32, 0xc5482c);
		DrawString(373, 270, "B", 0xffffff);
		DrawString(470, 270, "B Button(LongPress): ColorSwap", 0xffffff);

		//STARTボタン : PAUSE
		DrawCircleAA(373.f, 417.f, 38.f, 100, 0x1c2b3e, TRUE);
		DrawCircleAA(493.f, 417.f, 38.f, 100, 0x1c2b3e, TRUE);
		DrawBoxAA(360.f, 380.f, 500.f, 455.f, 0x1c2b3e, TRUE);
		DrawString(370, 395, "START", 0xffffff);
		DrawString(570, 395, "START Button : Pause", 0xffffff);

		//B(長押し中) + 十字カーソル：色の選択
		DrawCircleAA(390.f, 530.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(390.f, 530.f, 30.f, 32, 0xc5482c);
		DrawString(380, 507, "B", 0xffffff);
		DrawString(440, 507, "＋", 0xffffff);
		DrawCircleAA(540.f, 530.f, 50.f, 32, 0x1c2b3e);
		DrawBox(525, 490, 555, 575, 0x211d1e, TRUE);
		DrawBox(500, 520, 580, 545, 0x211d1e, TRUE);
		DrawString(630, 480, "B(Pressing)\nD-Pad", 0xffffff);	
		DrawString(925, 500, ":ColorSelect", 0xffffff);		

		//B(長押し中) + 左スティック：色の選択
		DrawCircleAA(390.f, 640.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(390.f, 640.f, 30.f, 32, 0xc5482c);
		DrawString(380, 615, "B", 0xffffff);
		DrawString(440, 615, "＋", 0xffffff);
		DrawCircleAA(540.f, 640.f, 50.f, 32, 0x1c2b3e);
		DrawCircleAA(540.f, 640.f, 30.f, 32, 0x525156);
		DrawString(630, 588, "B(Pressing)\nLeftStick", 0xffffff);	
		DrawString(925, 615, ":ColorSelect", 0xffffff);	

		break;

	case 2: //Player 何をしたらダメージを受けるのか、何回ダメージを受けたら死ぬのかの説明

		DrawLineAA(30.f, 240.f, 280.f, 240.f, 0xff0000, 5.0f);

		DrawBoxAA(360, 178, 562, 372, 0xffffff, FALSE, 2.0f);
		DrawBoxAA(362, 180, 560, 370, 0x555555, TRUE, 2.0f);

		DrawBoxAA(780, 68, 1202, 542, 0xffffff, FALSE, 2.0f);
		DrawBoxAA(782, 70, 1200, 540, 0x555555, TRUE, 2.0f);

		// Playerの体力
		for (int i = 0; i < p_hp; i++)
		{
			ResourceManager::DrawHeart({ 342.f + i * 50.f,120.f }, { 40.f,40.f });
		}

		HDrawPlayer(p_color);
		DrawString(385, 310, "Damage", 0xff0000);

		DrawString(930, 80, "Enemy", 0xffffff);
		HDrawDeer(DamageColor);
		HDrawFrog(DamageColor);
		HDrawBat(DamageColor);

		DrawString(825, 265, " Color Block", 0xffffff);
		DrawString(825, 400, " Damage Zone", 0xffffff);

		switch (DamageColor)
		{
		case RED:
			// 赤ブロック
			ResourceManager::StageBlockDraw(l_help_red, 0);
			//溶岩
			ResourceManager::StageAnimDraw(l_help_fire, 3);
			break;

		case BLUE:
			// 青ブロック
			ResourceManager::StageBlockDraw(l_help_blue, 1);
			//水
			ResourceManager::StageAnimDraw(l_help_water, 5);
			break;

		case GREEN:
			// 緑ブロック
			DrawBoxAA(965.f, 390.f - 50, 1005.f, 430.f - 50, GREEN, TRUE);
			// 緑ブロックの点々
			DrawBoxAA(975.f, 410.f - 50, 980.f, 415.f - 50, 0x00ee00, TRUE);
			DrawBoxAA(990.f, 425.f - 50, 995.f, 430.f - 50, 0x00ee00, TRUE);
			DrawBoxAA(995.f, 405.f - 50, 1000.f, 410.f - 50, 0x00ee00, TRUE);

			//木
			ResourceManager::StageAnimDraw(l_help_wood, 4);

			//竹
			DrawBoxAA(l_help_bamboo.x + 3, l_help_bamboo.y, l_help_bamboo.x + BOX_WIDTH - 3, l_help_bamboo.y + BOX_HEIGHT, 0x00cc00, true);
			DrawBoxAA(l_help_bamboo.x + 2, l_help_bamboo.y, l_help_bamboo.x + BOX_WIDTH - 2, l_help_bamboo.y + 2, 0x00ff00, true);
			DrawBoxAA(l_help_bamboo.x + 2, l_help_bamboo.y + BOX_HEIGHT, l_help_bamboo.x + BOX_WIDTH - 2, l_help_bamboo.y + BOX_HEIGHT - 2, 0x00ff00, true);
			DrawBoxAA(l_help_bamboo.x + 10, l_help_bamboo.y + 2, l_help_bamboo.x + 13, l_help_bamboo.y + BOX_HEIGHT - 2, 0x00ee00, true);

		default:
			break;
		}

		SetFontSize(100);
		DrawString(380, 570, "5 Hit", 0xffffff);
		if (Gameoverflg == false && g_cnt != 0)
		{
			DrawString(600, 570, "「GAME OVER」", 0xff0000);
		}
		//不等号
		if (DamageColor == RED && p_color == RED)
		{
			DrawFormatString(650, 225, 0xffffff, "%c", sign[1]);
		}
		else
		{
			DrawFormatString(650, 225, 0xffffff, "%c", sign[0]);
		}

		SetFontSize(49);

		break;

	case 3:	//色 横並びで説明 
		
		DrawLineAA(30.f, 380.f, 280.f, 380.f, 0xff0000, 5.0f);

		DrawBoxAA(340, 98, 542, 182, 0xffffff, FALSE, 2.0f);
		DrawBoxAA(342, 100, 540, 180, 0x555555, TRUE, 2.0f);

		// 色の交換できるブロックの説明
		ResourceManager::StageBlockDraw(l_help_red, 0);
		ResourceManager::StageBlockDraw(l_help_blue, 1);

		DrawBoxAA(480.f, 60.f + 60, 520.f, 100.f + 60, GREEN, TRUE);
		DrawBoxAA(490.f, 80.f + 60, 495.f, 85.f + 60, 0x00ee00, TRUE);
		DrawBoxAA(505.f, 95.f + 60, 510.f, 100.f + 60, 0x00ee00, TRUE);
		DrawBoxAA(510.f, 75.f + 60, 515.f, 80.f + 60, 0x00ee00, TRUE);
		
		DrawString(620, 100, " Color Block\n(ColorSwap OK)", 0xffffff);

		DrawBoxAA(340, 298, 612, 402, 0xffffff, FALSE, 2.0f);
		DrawBoxAA(342, 300, 610, 400, 0x555555, TRUE, 2.0f);

		// ダメージゾーンの説明
		ResourceManager::StageAnimDraw(l_help_fire, 3);
		ResourceManager::StageAnimDraw(l_help_water, 5);
		ResourceManager::StageAnimDraw(l_help_wood, 4);

		DrawBoxAA(l_help_bamboo.x + 3, l_help_bamboo.y, l_help_bamboo.x + BOX_WIDTH - 3, l_help_bamboo.y + BOX_HEIGHT, 0x00cc00, true);
		DrawBoxAA(l_help_bamboo.x + 2, l_help_bamboo.y, l_help_bamboo.x + BOX_WIDTH - 2, l_help_bamboo.y + 2, 0x00ff00, true);
		DrawBoxAA(l_help_bamboo.x + 2, l_help_bamboo.y + BOX_HEIGHT, l_help_bamboo.x + BOX_WIDTH - 2, l_help_bamboo.y + BOX_HEIGHT - 2, 0x00ff00, true);
		DrawBoxAA(l_help_bamboo.x + 10, l_help_bamboo.y + 2, l_help_bamboo.x + 13, l_help_bamboo.y + BOX_HEIGHT - 2, 0x00ee00, true);

		DrawString(620, 310, " Damage Zone\n(ColorSwap NG)", 0xffffff);

		DrawBoxAA(415, 568, 532, 672, 0xffffff, FALSE, 2.0f);
		DrawBoxAA(417, 570, 530, 670, 0x555555, TRUE, 2.0f);

		// 中間地点の説明
		DrawTriangleAA(l_help_respawn.x + (BOX_WIDTH / 2), l_help_respawn.y,l_help_respawn.x + BOX_WIDTH, l_help_respawn.y + (BOX_HEIGHT / 4),l_help_respawn.x + (BOX_WIDTH / 2), l_help_respawn.y + (BOX_HEIGHT / 2), 0xffffff, TRUE);

		DrawBoxAA(l_help_respawn.x + (BOX_WIDTH / 2) - 2, l_help_respawn.y, l_help_respawn.x + (BOX_WIDTH / 2) + 2, l_help_respawn.y + BOX_HEIGHT, 0xffffff, TRUE);
		DrawBoxAA(l_help_respawn.x, l_help_respawn.y + BOX_HEIGHT - 4, l_help_respawn.x + BOX_WIDTH, l_help_respawn.y + BOX_HEIGHT, 0xffffff, TRUE);

		DrawStringF(620.f, l_help_respawn.y, "Respawn Point", 0xffffff);

		break;
	case 4: //属性

		//カーソル描画
		DrawLineAA(30.f, 520.f, 280.f, 520.f, 0xff0000, 5.0f);

		DrawCircleAA(770.f, 140.f, 100.f, 32, 0xcc0000, TRUE);
		DrawLineAA(690.f, 244.f, 580.f, 445.f, 0xffffff, 10.0f);
		DrawLineAA(648.f, 250.f, 690.f, 246.f, 0xffffff, 10.0f);
		DrawLineAA(687.f, 241.f, 695.f, 285.f, 0xffffff, 10.0f);

		DrawCircleAA(510.f, 550.f, 100.f, 32, 0x3c78d8, TRUE);

		DrawLineAA(618.f, 554.f, 930.f, 554.f, 0xffffff, 10.0f);
		DrawLineAA(620.f, 555.f, 652.f, 521.f, 0xffffff, 10.0f);
		DrawLineAA(620.f, 555.f, 655.f, 576.f, 0xffffff, 10.0f);

		DrawCircleAA(1060.f, 550.f, 100.f, 32, 0x6aa84f, TRUE);
		
		DrawLineAA(840.f, 240.f, 980.f, 445.f, 0xffffff, 10.0f);
		DrawLineAA(982.f, 440.f, 980.f, 380.f, 0xffffff, 10.0f);
		DrawLineAA(918.f, 426.f, 973.f, 440.f, 0xffffff, 10.0f);

		SetFontSize(30);
		DrawString(710, 110, "  Red\n(Fire)", 0xFFFF55);			//赤\n(火属性)
		DrawString(450, 525, "  Blue\n(Water)", 0xFFFF55);			//青\n(水属性)
		DrawString(1000, 520, " Green\n(Wood)", 0xFFFF55);			//緑\n(木属性)

		DrawString(730, 580, "Damage", 0x00ff00);
		DrawString(500, 300, "Damage", 0x0000ff);
		DrawString(970, 300, "Damage", 0xff0000);

		break;
	case 5: //タイトルへ
		
		DrawLineAA(30.f, 683.f, 280.f, 683.f, 0xff0000, 5.0f);
		//Press B button
		DrawString(630, 330, "Press B button", 0xffffff);
		if (button_draw == false)
		{
			//ボタンイメージ描画
			DrawCircleAA(550, 360, 37.5, 100, 0xff0000, FALSE);
			DrawStringF(550 - 10, 290 + 40, "B", 0xff0000);

			//押してる演出の描画
			DrawLineAA(550 - 40, 290 + 75, 550 - 60, 290 + 70, 0xffffff, TRUE);
			DrawLineAA(550 - 40, 290 + 58, 550 - 60, 290 + 53, 0xffffff, TRUE);
			DrawLineAA(550 + 40, 290 + 75, 550 + 60, 290 + 70, 0xffffff, TRUE);
			DrawLineAA(550 + 40, 290 + 58, 550 + 60, 290 + 53, 0xffffff, TRUE);
		}
		else
		{
			DrawCircleAA(550, 290 + 55, 37.5, 100, 0xff0000, TRUE);
			DrawCircleAA(550, 290 + 70, 37.5, 100, 0xff0000, TRUE);
			DrawBoxAA(550 - 20, 290 + 55, 550 + 20, 290 + 70, 0xff0000, TRUE);
			DrawStringF(550 - 10, 290 + 25, "B", 0x000000);
		}
		break;
	default:
		break;
	}

	
}

void Help::HDrawPlayer(int c_num)const
{
	//帽子　中央
	DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 20, player_location.y + 20, player_location.x + 40, player_location.y + 20, p_color, true);
	DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 20, player_location.y + 20, player_location.x + 40, player_location.y + 20, 0x000000, false);
	//帽子　右側	  									   					  						  						  		 
	DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 40, player_location.y + 20, player_location.x + 52, player_location.y + 15, p_color, true);
	DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 40, player_location.y + 20, player_location.x + 52, player_location.y + 15, 0x000000, false);
	//帽子　左側	  									   					  						  						  		 
	DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 8, player_location.y + 15, player_location.x + 20, player_location.y + 20, p_color, true);
	DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 8, player_location.y + 15, player_location.x + 20, player_location.y + 20, 0x000000, false);

	//頭
	ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+76, 23, 15, player_location.x, player_location.y, 0, p_color, true);
	ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+76, 23, 15, player_location.x, player_location.y, 0, 0x000000, false);

	//目
	ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) - 6, player_location.y - (PLAYER_HEIGHT)+76, 6, 7, player_location.x, player_location.y, 0, 0x000000, true);

	//首
	ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+62, 10, 5, player_location.x, player_location.y, 0, p_color, true);
	ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+62, 10, 5, player_location.x, player_location.y, 0, 0x000000, false);

	//胴体																					
	ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+37, 21, 37, player_location.x, player_location.y, 0, p_color, true);
	ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+37, 21, 37, player_location.x, player_location.y, 0, 0x000000, false);

	//バッグ
	ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) + 15, player_location.y - (PLAYER_HEIGHT)+40, 5, 23, player_location.x, player_location.y, 0, p_color, true);
	ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) + 15, player_location.y - (PLAYER_HEIGHT)+40, 5, 23, player_location.x, player_location.y, 0, 0x000000, false);
	ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) + 15, player_location.y - (PLAYER_HEIGHT)+40, 3, 15, player_location.x, player_location.y, 0, 0x000000, true);

	//腕
	ResourceManager::DrawRotaBox(player_location.x + 25, player_location.y + 55, 28, 7, player_location.x + 35, player_location.y + 55, 0, p_color, true);
	ResourceManager::DrawRotaBox(player_location.x + 25, player_location.y + 55, 28, 7, player_location.x + 35, player_location.y + 55, 0, 0x000000, false);

	//足												 														
	ResourceManager::DrawRotaBox(player_location.x + 30, player_location.y + 70, 7, 27, player_location.x + 30, player_location.y + 80, 0, p_color, true);
	ResourceManager::DrawRotaBox(player_location.x + 30, player_location.y + 70, 7, 27, player_location.x + 30, player_location.y + 80, 0, 0x000000, false);
}

void Help::HDrawDeer(int c_num)const
{
	//鹿
	//頭
	ResourceManager::DrawRotaBox(l_Deer.x + 16.0f, l_Deer.y + 10.0f , 30.0f, 20.0f, l_Deer.x + 16.0f, l_Deer.y + 10.0f, 0, c_num, true);
	ResourceManager::DrawRotaBox(l_Deer.x + 16.0f, l_Deer.y + 10.0f, 30.0f, 20.0f, l_Deer.x + 16.0f, l_Deer.y + 10.0f, 0, 0x000000, FALSE);
	//目
	ResourceManager::DrawRotaBox(l_Deer.x + 8.0f, l_Deer.y + 10.0f, 8.0f, 9.0f, l_Deer.x + 8.0f, l_Deer.y + 10.0f, 0, 0x000000, true);
	ResourceManager::DrawRotaBox(l_Deer.x + 8.0f, l_Deer.y + 10.0f, 8.0f, 9.0f, l_Deer.x + 8.0f, l_Deer.y + 10.0f, 0, 0x000000, FALSE);
	//首
	ResourceManager::DrawRotaBox(l_Deer.x + 25.0f, l_Deer.y + 38.0f, 13.0f, 24.0f, l_Deer.x + 25.0f, l_Deer.y + 38.0f, 0, c_num, true);
	ResourceManager::DrawRotaBox(l_Deer.x + 25.0f, l_Deer.y + 38.0f, 13.0f, 24.0f, l_Deer.x + 25.0f, l_Deer.y + 38.0f, 0, 0x000000, FALSE);
	//胴体
	ResourceManager::DrawRotaBox(l_Deer.x + 53.0f, l_Deer.y + 63.0f, 65.0f, 15.0f, l_Deer.x + 53.0f, l_Deer.y + 63.0f, 0, c_num, true);
	ResourceManager::DrawRotaBox(l_Deer.x + 53.0f, l_Deer.y + 63.0f, 65.0f, 15.0f, l_Deer.x + 53.0f, l_Deer.y + 63.0f, 0, 0x000000, FALSE);
	//足　左から
	ResourceManager::DrawRotaBox(l_Deer.x + 27.0f, l_Deer.y + 88.0f , 10.0f, 25.0f, l_Deer.x + 27.0f, l_Deer.y + 88.0f, 0, c_num, true);
	ResourceManager::DrawRotaBox(l_Deer.x + 27.0f, l_Deer.y + 88.0f, 10.0f, 25.0f, l_Deer.x + 27.0f, l_Deer.y + 88.0f, 0, 0x000000, FALSE);
	ResourceManager::DrawRotaBox(l_Deer.x + 43.0f, l_Deer.y + 88.0f , 10.0f, 25.0f, l_Deer.x + 43.0f, l_Deer.y + 88.0f, 0, c_num, true);
	ResourceManager::DrawRotaBox(l_Deer.x + 43.0f, l_Deer.y + 88.0f, 10.0f, 25.0f, l_Deer.x + 43.0f, l_Deer.y + 88.0f, 0, 0x000000, FALSE);
	ResourceManager::DrawRotaBox(l_Deer.x + 68.0f, l_Deer.y + 88.0f , 10.0f, 25.0f, l_Deer.x + 68.0f, l_Deer.y + 88.0f, 0, c_num, true);
	ResourceManager::DrawRotaBox(l_Deer.x + 68.0f, l_Deer.y + 88.0f, 10.0f, 25.0f, l_Deer.x + 68.0f, l_Deer.y + 88.0f, 0, 0x000000, FALSE);
	ResourceManager::DrawRotaBox(l_Deer.x + 83.0f, l_Deer.y + 88.0f , 10.0f, 25.0f, l_Deer.x + 83.0f, l_Deer.y + 88.0f, 0, c_num, true);
	ResourceManager::DrawRotaBox(l_Deer.x + 83.0f, l_Deer.y + 88.0f, 10.0f, 25.0f, l_Deer.x + 83.0f, l_Deer.y + 88.0f, 0, 0x000000, FALSE);
}

void Help::HDrawBat(int c_num)const
{
	//蝙蝠
	//左耳
	DrawTriangleAA(l_Bat.x + 46, l_Bat.y, l_Bat.x + 46, l_Bat.y + 19, l_Bat.x + 55, l_Bat.y + 9, c_num, TRUE);
	DrawTriangleAA(l_Bat.x + 46, l_Bat.y, l_Bat.x + 46, l_Bat.y + 19, l_Bat.x + 55, l_Bat.y + 9, 0x000000, FALSE);
	//右耳
	DrawTriangleAA(l_Bat.x + 69, l_Bat.y, l_Bat.x + 69, l_Bat.y + 19, l_Bat.x + 60, l_Bat.y + 9, c_num, TRUE);
	DrawTriangleAA(l_Bat.x + 69, l_Bat.y, l_Bat.x + 69, l_Bat.y + 19, l_Bat.x + 60, l_Bat.y + 9, 0x000000, FALSE);
	//右羽
	DrawTriangleAA(l_Bat.x + 66, l_Bat.y + 33, l_Bat.x + 97, l_Bat.y + 7, l_Bat.x + 117, l_Bat.y + 65, c_num, TRUE);
	DrawTriangleAA(l_Bat.x + 66, l_Bat.y + 33, l_Bat.x + 97, l_Bat.y + 7, l_Bat.x + 96 , l_Bat.y + 61, c_num, TRUE);
	DrawTriangleAA(l_Bat.x + 65, l_Bat.y + 33, l_Bat.x + 97, l_Bat.y + 7, l_Bat.x + 80 , l_Bat.y + 57, c_num, TRUE);
	//左羽
	DrawTriangleAA(l_Bat.x + 48, l_Bat.y + 33, l_Bat.x + 20, l_Bat.y + 7, l_Bat.x - 2, l_Bat.y + 65, c_num, TRUE);
	DrawTriangleAA(l_Bat.x + 48, l_Bat.y + 33, l_Bat.x + 20, l_Bat.y + 7, l_Bat.x + 18, l_Bat.y + 61, c_num, TRUE);
	DrawTriangleAA(l_Bat.x + 50, l_Bat.y + 33, l_Bat.x + 20, l_Bat.y + 7, l_Bat.x + 34, l_Bat.y + 57, c_num, TRUE);
	// 頭
	DrawQuadrangleAA(l_Bat.x + 57, l_Bat.y + 12, l_Bat.x + 69, l_Bat.y + 23, l_Bat.x + 57, l_Bat.y + 30, l_Bat.x + 46, l_Bat.y + 23, c_num, TRUE);
	// 胴体
	DrawQuadrangleAA(l_Bat.x + 57, l_Bat.y + 34, l_Bat.x + 69, l_Bat.y + 52, l_Bat.x + 57, l_Bat.y + 73, l_Bat.x + 46, l_Bat.y + 52, c_num, TRUE);
}

void Help::HDrawFrog(int c_num)const
{
	//蛙
	//胴体
	ResourceManager::DrawRotaBox(l_Frog.x + (50 / 2), l_Frog.y + (50 / 2), 50, 50 / 2, l_Frog.x + (50 / 2), l_Frog.y + (50 / 2), 180, c_num, TRUE);
	ResourceManager::DrawRotaBox(l_Frog.x + (50 / 2), l_Frog.y + (50 / 2), 50, 50 / 2, l_Frog.x + (50 / 2), l_Frog.y + (50 / 2), 180, 0x000000, FALSE);
	//付け根側後ろ足
	ResourceManager::DrawRotaBox(l_Frog.x + 50 - 10, l_Frog.y + 50 - 20 , 30, 10, l_Frog.x + (50 / 2), l_Frog.y + (50 / 2), 180, c_num, TRUE);
	ResourceManager::DrawRotaBox(l_Frog.x + 50 - 10, l_Frog.y + 50 - 20 , 30, 10, l_Frog.x + (50 / 2), l_Frog.y + (50 / 2), 180, 0x000000, FALSE);
	//後ろ足先端
	ResourceManager::DrawRotaBox(l_Frog.x + 50 - 10, l_Frog.y + 50 - 10 , 40, 10, l_Frog.x + (50 / 2), l_Frog.y + (50 / 2), 180, c_num, TRUE);
	ResourceManager::DrawRotaBox(l_Frog.x + 50 - 10, l_Frog.y + 50 - 10 , 40, 10, l_Frog.x + (50 / 2), l_Frog.y + (50 / 2), 180, 0x000000, FALSE);

	ResourceManager::DrawRotaBox(l_Frog.x + 10, l_Frog.y + 50 - 10 , 10, 15, l_Frog.x + (50 / 2), l_Frog.y + (50 / 2), 180, c_num, TRUE);
	ResourceManager::DrawRotaBox(l_Frog.x + 10, l_Frog.y + 50 - 10 , 10, 15, l_Frog.x + (50 / 2), l_Frog.y + (50 / 2), 180, 0x000000, FALSE);
	//目
	ResourceManager::DrawRotaBox(l_Frog.x, l_Frog.y + 20, 10, 10, l_Frog.x + (50 / 2), l_Frog.y + (50 / 2), 180, 0xffffff, TRUE);
	ResourceManager::DrawRotaBox(l_Frog.x, l_Frog.y + 20, 10, 10, l_Frog.x + (50 / 2), l_Frog.y + (50 / 2), 180, 0x000000, FALSE);
}