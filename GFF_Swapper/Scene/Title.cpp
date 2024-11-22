#include "Title.h"
#include "DxLib.h"
#include "GameMain.h"
#include "Help.h"
#include "Option.h"
#include "End.h"
#include "../Utility/PadInput.h"
#include"../Utility/ResourceManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

Title::Title() :frame(0), menu_location{ 0,0 }, menu_size{ 0,0 }, player_location{ 0,0 }, player_color(0xff0000),cursor_location{ 0,0 }, draw_stick_location{ 0,0 }, draw_stick_shift{ 0,0 }, anim_start{ 0,0 }, current_menu(0), swap_anim_flg(false), swap_anim_timer(0), stick_angle(0.0f), button_draw(false),bg_handle(0), circleAng(0), rise_se(0), cursor_se(0)
{
}

Title::~Title()
{
}

void Title::Initialize()
{
	player_location = { (SCREEN_WIDTH / 2) - (PLAYER_WIDTH / 2), SCREEN_HEIGHT - PLAYER_HEIGHT-75 };
	menu_location[0] = { 250,300 };
	menu_location[1] = { 465,250 };
	menu_location[2] = { 665,250 };
	menu_location[3] = { 900,300 };
	draw_stick_location = { 300,600 };
	menu_size[0] = { 150,75 };
	menu_size[1] = { 150,75 };
	menu_size[2] = { 150,75 };
	menu_size[3] = { 150,75 };

	for (int i = 0; i < 7; i++)
	{
		logo_location[i] = { 0,0 };
		for (int j = 0; j < i; j++)
		{
			logo_location[i].x += 90;
		}
	}

	//GraphFilter(ResourceManager::GetGraph(logo_img), DX_GRAPH_FILTER_GAUSS, 16, 1400);

	swap_se = ResourceManager::SetSound("Resource/Sounds/Effect/swap.wav");
	rise_se = ResourceManager::SetSound("Resource/Sounds/System/rise.wav");
	cursor_se = ResourceManager::SetSound("Resource/Sounds/Player/cursor.wav");
	ResourceManager::StartSound(ResourceManager::SetSound("Resource/Sounds/BGM/Title.wav"), TRUE);

	for (int i = 0; i < BG_BLOCK_WIDTH_NUM; i++)
	{
		for (int j = 0; j < BG_BLOCK_HEIGHT_NUM; j++)
		{
			bg[i][j].flg = true;
			bg[i][j].location = { (float)i * 40,(float)j * 40 };
			bg[i][j].erea = { 40,40 };
			bg[i][j].move_flg = false;
			bg[i][j].move_goal = { 0,0 };
			bg[i][j].move_speed = 6;
			bg[i][j].color = GetRandColor();
			bg[i][j].move_rad = 0.0f;
			bg[i][j].anim_size = 0;
		}
	}
	//プレイヤーの色をランダムで決める
	player_color = color_list[GetRand(2)];
	bg_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Title::Finalize()
{
	ResourceManager::StopSound(ResourceManager::SetSound("Resource/Sounds/BGM/Title.wav"));
}

AbstractScene* Title::Update()
{
	if (++frame > 6000)
	{
		frame = 0;
	}

	if (frame % 10 == 0)
	{
		button_draw = !button_draw;
	}
	stick_angle += 0.05f;
	if (stick_angle > 1)
	{
		stick_angle = 0;
	}

	//背景の更新
	bgUpdate();

	//演出中もしくはエンドアニメーション中は更新しない
	if (swap_anim_flg == false)
	{
		//選択メニュー更新（スティック）
		if (current_menu != 0 && 
			cursor_location.x < player_location.x-50 && 
			cursor_location.y < player_location.y + (PLAYER_HEIGHT / 2) - 30)
		{
			current_menu = 0;
			ResourceManager::StartSound(cursor_se);
		}
		else if (current_menu != 1 && 
				 cursor_location.x > player_location.x-50 && 
				 cursor_location.x < player_location.x + PLAYER_WIDTH && 
				 cursor_location.y < player_location.y + (PLAYER_HEIGHT / 2) - 30)
		{
			current_menu = 1;
			ResourceManager::StartSound(cursor_se);
		}
		else if (current_menu != 2 &&
				 cursor_location.x > player_location.x + PLAYER_WIDTH &&
				 cursor_location.x < player_location.x + PLAYER_WIDTH+50 &&
				 cursor_location.y < player_location.y + (PLAYER_HEIGHT / 2) - 30)
		{
			current_menu = 2;
			ResourceManager::StartSound(cursor_se);
		}
		else if (current_menu != 3 && 
				 cursor_location.x > player_location.x + PLAYER_WIDTH+50 && 
				 cursor_location.y < player_location.y + (PLAYER_HEIGHT / 2) - 30)
		{
			current_menu = 3;
			ResourceManager::StartSound(cursor_se);
		}
		//カーソル更新
		cursor_location.x = player_location.x + (PLAYER_WIDTH / 2) + PadInput::TipLStick(STICKL_X) * 100;
		cursor_location.y = player_location.y + (PLAYER_HEIGHT / 2) - PadInput::TipLStick(STICKL_Y) * 100;

		//選択メニュー更新（十字キー）
		if (PadInput::OnButton(XINPUT_BUTTON_DPAD_LEFT))
		{
			if (--current_menu < 0)
			{
				current_menu = MENU_NUM-1;
			}
			ResourceManager::StartSound(cursor_se);
		}
		if (PadInput::OnButton(XINPUT_BUTTON_DPAD_RIGHT))
		{
			if (++current_menu > MENU_NUM-1)
			{
				current_menu = 0;
			}
			ResourceManager::StartSound(cursor_se);
		}
	}

	draw_stick_shift.x = cosf(stick_angle * (float)M_PI * 2) * 5;
	draw_stick_shift.y = sinf(stick_angle * (float)M_PI * 2) * 5;


	

	//Bボタンでアニメーションを再生
	if (frame >10 && PadInput::OnRelease(XINPUT_BUTTON_B))
	{
		if (swap_anim_flg == false)
		{
			swap_anim_flg = true;
			anim_start = menu_location[current_menu];
			ResourceManager::StartSound(rise_se);
		}
		//再生中に押されたらスキップ
		else
		{
			swap_anim_timer = 255;
			ResourceManager::StopSound(rise_se);
		}
	}

	//演出開始
	if (swap_anim_flg == true)
	{
		//演出１つ目
		//if (swap_anim_timer < 30)
		//{
		//	menu_location[current_menu].x += (player_location.x - anim_start.x-(menu_size[current_menu].x/4)) / 30;
		//	menu_location[current_menu].y += (player_location.y - anim_start.y) / 30;
		//}
		//else
		//{
		//	menu_location[current_menu].y -= (SCREEN_HEIGHT / 30);
		//	menu_size[current_menu].y += (SCREEN_HEIGHT /15);
		//	menu_location[current_menu].x -= (SCREEN_WIDTH / 30);
		//	menu_size[current_menu].x += (SCREEN_WIDTH / 15);
		//}
				//演出開始から一定時間経ったら遷移
		//if (++swap_anim_timer > 60)
		//{
		//	swap_anim_flg = false;
		//	swap_anim_timer = 0;
		//	menu_location[current_menu] = anim_start;
		//	menu_size[current_menu] = { 75,150 };
		//	switch (current_menu)
		//	{
		//	case 0:
		//		//ゲームメイン
		//		return new GameMain(0);
		//		break;
		//	case 1:
		//		//ヘルプ画面
		//		printfDx("ヘルプ画面");
		//		return new Help();
		//		break;
		//	case 2:
		//		//ゲーム終了
		//		printfDx("ゲーム終了");
		//		/*return new End();*/
		//		end_game_flg = true;
		//		title_image_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
		//		GetDrawScreenGraph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, title_image_handle);
		//		break;
		//	}
		//}
	
		//演出二つ目
		swap_anim_timer++;
		int w, h, try_num=0;
		for (int i = 0; i < 3; i++)
		{
			do {
				w = GetRand(BG_BLOCK_WIDTH_NUM - 1);
				h = GetRand(BG_BLOCK_HEIGHT_NUM - 1);
				try_num++;
			} while (bg[w][h].flg == false && try_num < 100);
			bg[w][h].flg = false;
			if (try_num >= 100 || swap_anim_timer > 255)
			{
				swap_anim_flg = false;
				swap_anim_timer = 0;
				menu_location[current_menu] = anim_start;
				menu_size[current_menu] = { 75,150 };
				switch (current_menu)
				{
				case 0:
					//ゲームメイン
					return new GameMain(0);
					break;
				case 1:
					//ヘルプ画面
					return new Help();
					break;
				case 2:
					//オプション画面
					return new Option();
					break;
				case 3:
					//ゲーム終了
					return new End();
					break;
				}
			}
		}
	}

	if (circleAng++ >= 360.f) {
		circleAng = 0.f;
	}

	return this;
}

void Title::Draw()const
{
	//変更前のフォントサイズを保存
	int old_size = GetFontSize();
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, TRUE);
		//動いていないブロックを先に描画
		for (int i = 0; i < BG_BLOCK_WIDTH_NUM; i++)
		{
			for (int j = 0; j < BG_BLOCK_HEIGHT_NUM; j++)
			{
				if (bg[i][j].move_flg == false && bg[i][j].flg == true)
				{
					DrawBoxAA(bg[i][j].location.x- bg[i][j].anim_size, 
						bg[i][j].location.y- bg[i][j].anim_size, 
						bg[i][j].location.x + bg[i][j].erea.x + bg[i][j].anim_size, 
						bg[i][j].location.y + bg[i][j].erea.y+ bg[i][j].anim_size, bg[i][j].color, TRUE);
					DrawBoxAA(bg[i][j].location.x- bg[i][j].anim_size, 
						bg[i][j].location.y- bg[i][j].anim_size,
						bg[i][j].location.x + bg[i][j].erea.x+ bg[i][j].anim_size, 
						bg[i][j].location.y + bg[i][j].erea.y+ bg[i][j].anim_size, 0x444444, FALSE);
				}
			}
		}
		//動いているブロックを描画
		for (int i = 0; i < BG_BLOCK_WIDTH_NUM; i++)
		{
			for (int j = 0; j < BG_BLOCK_HEIGHT_NUM; j++)
			{
				if (bg[i][j].move_flg == true && bg[i][j].flg == true)
				{
					DrawBoxAA(bg[i][j].location.x - bg[i][j].anim_size,
						bg[i][j].location.y - bg[i][j].anim_size,
						bg[i][j].location.x + bg[i][j].erea.x + bg[i][j].anim_size,
						bg[i][j].location.y + bg[i][j].erea.y + bg[i][j].anim_size, bg[i][j].color+0x111111, TRUE);
					DrawBoxAA(bg[i][j].location.x - bg[i][j].anim_size,
						bg[i][j].location.y - bg[i][j].anim_size,
						bg[i][j].location.x + bg[i][j].erea.x + bg[i][j].anim_size,
						bg[i][j].location.y + bg[i][j].erea.y + bg[i][j].anim_size, 0x555555, FALSE);
				}
			}
		}
		GetDrawScreenGraph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg_handle);	//背景をハンドルに保存
		ClearDrawScreen();													// 画面の初期化
		GraphFilter(bg_handle, DX_GRAPH_FILTER_GAUSS, 16, 1400);			//保存した背景にぼかしをかける
		DrawGraph(0, 0, bg_handle, TRUE);									//背景描画


		//ロゴ画像描画
		SetFontSize(144);
		for (int i = 0; i < 7; i++)
		{
			if (swap_anim_flg == false)
			{
				DrawFormatStringF(logo_location[i].x + 325, logo_location[i].y + 60, logo_color[i], "%s", logo_string[i]);
			}
			else
			{
				DrawFormatStringF(logo_location[i].x + 325+GetRand(10)-5, logo_location[i].y + 60 + GetRand(10)-5, logo_color[i], "%s", logo_string[i]);

			}
		}
		if (swap_anim_flg == true)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, swap_anim_timer);
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		else
		{
			//プレイヤー画像描画
			DrawPlayer(current_menu);
			DrawBoxAA(player_location.x - 50, player_location.y + PLAYER_HEIGHT, player_location.x + PLAYER_WIDTH + 50, SCREEN_HEIGHT, 0x000000, TRUE);
			DrawBoxAA(player_location.x - 50, player_location.y + PLAYER_HEIGHT, player_location.x + PLAYER_WIDTH + 50, SCREEN_HEIGHT + 5, 0xffffff, FALSE);

			//説明UI描画
			DrawCircleAA(draw_stick_location.x, draw_stick_location.y, 15, 100, 0x000000, TRUE);
			DrawCircleAA(draw_stick_location.x, draw_stick_location.y, 15, 100, 0x666666, FALSE);
			DrawCircleAA(draw_stick_location.x + draw_stick_shift.x, draw_stick_location.y + draw_stick_shift.y, 12, 100, 0x666666, TRUE);
			SetFontSize(24);
			DrawStringF(draw_stick_location.x + 25, draw_stick_location.y - 10, "Left Stick", 0xffffff);
			DrawStringF(draw_stick_location.x + 50, draw_stick_location.y + 20, "&", 0xffffff);
			DrawStringF(draw_stick_location.x + 35, draw_stick_location.y + 45, "B Button", 0xffffff);

			SetFontSize(16);
			if (button_draw == false)
			{
				//ボタンイメージ描画
				DrawCircleAA(draw_stick_location.x, draw_stick_location.y + 60, 12, 100, 0xff0000, FALSE);
				DrawStringF(draw_stick_location.x - 3, draw_stick_location.y + 53, "B", 0xff0000);

				//押してる演出の描画
				DrawLineAA(draw_stick_location.x - 20, draw_stick_location.y + 63, draw_stick_location.x - 25, draw_stick_location.y + 60, 0xffffff, TRUE);
				DrawLineAA(draw_stick_location.x - 20, draw_stick_location.y + 58, draw_stick_location.x - 25, draw_stick_location.y + 53, 0xffffff, TRUE);
				DrawLineAA(draw_stick_location.x + 20, draw_stick_location.y + 63, draw_stick_location.x + 25, draw_stick_location.y + 60, 0xffffff, TRUE);
				DrawLineAA(draw_stick_location.x + 20, draw_stick_location.y + 58, draw_stick_location.x + 25, draw_stick_location.y + 53, 0xffffff, TRUE);
			}
			else
			{
				DrawCircleAA(draw_stick_location.x, draw_stick_location.y + 55, 12, 100, 0xff0000, TRUE);
				DrawCircleAA(draw_stick_location.x, draw_stick_location.y + 60, 12, 100, 0xff0000, TRUE);
				DrawBoxAA(draw_stick_location.x - 12, draw_stick_location.y + 55, draw_stick_location.x + 12, draw_stick_location.y + 60, 0xff0000, TRUE);
				DrawStringF(draw_stick_location.x - 3, draw_stick_location.y + 48, "B", 0x000000);
			}

			SetFontSize(48);
			//メニュー項目の描画
			for (int i = 0; i < MENU_NUM; i++)
			{
				if (swap_anim_timer <= 30)
				{
					DrawBoxAA(menu_location[i].x, menu_location[i].y, menu_location[i].x + menu_size[i].x, menu_location[i].y + menu_size[i].y, 0x000000, TRUE);
					DrawBoxAA(menu_location[i].x+frame%60/30, menu_location[i].y + frame % 60 / 30, menu_location[i].x + menu_size[i].x - frame % 60 / 30, menu_location[i].y + menu_size[i].y - frame % 60 / 30, 0xffffff, FALSE);
				}
				else if (current_menu == i)
				{
					DrawBoxAA(menu_location[i].x, menu_location[i].y, menu_location[i].x + menu_size[i].x, menu_location[i].y + menu_size[i].y, 0x000000, TRUE);
					DrawBoxAA(menu_location[i].x, menu_location[i].y, menu_location[i].x + menu_size[i].x, menu_location[i].y + menu_size[i].y, 0xffffff, FALSE);
				}
				if (swap_anim_timer <= 30)
				{
					DrawFormatStringF(menu_location[i].x + (menu_size[i].x / 2) - (GetDrawStringWidth(menu_string[i], (int)strlen(menu_string[i])) / 2), menu_location[i].y + (menu_size[i].y / 2) - 24, 0xffffff, "%s", menu_string[i]);
					//選択中のメニューに交換カーソルを出す
					if (current_menu == i)
					{
						DrawCircleAA(menu_location[i].x + (menu_size[i].x / 2), menu_location[i].y + (menu_size[i].y / 2), 70.f, 40, 0xffff00, FALSE, 4.f * 1.75f);

						Vector2D base;
						base.x = menu_location[i].x + (menu_size[i].x / 2);
						base.y = menu_location[i].y + (menu_size[i].y / 2);

						Vector2D l[3];
						l[0].x = base.x;
						l[0].y = base.y - 70;

						l[0] = RotationLocation(base, l[0], (float)(circleAng * M_PI / 180));

						l[1] = RotationLocation(base, l[0], (float)(120.f * M_PI / 180));

						l[2] = RotationLocation(base, l[0], (float)(240.f * M_PI / 180));


						DrawCircleAA(l[0].x, l[0].y, 15.f * 1.75f, 32, 0xcc0000, TRUE);
						DrawCircleAA(l[1].x, l[1].y, 15.f * 1.75f, 32, 0x3c78d8, TRUE);
						DrawCircleAA(l[2].x, l[2].y, 15.f * 1.75f, 32, 0x6aa84f, TRUE);
					}
				}
			}
		}

	//以前のサイズに戻す
	SetFontSize(old_size);
}

void Title::bgUpdate()
{
	//背景の交換
	if (frame % 120 == 0 || PadInput::OnRelease(XINPUT_BUTTON_A))
	{
		int w1, h1, w2, h2,try_num=0;
		do {
			w1 = GetRand(BG_BLOCK_WIDTH_NUM - 1);
			h1 = GetRand(BG_BLOCK_HEIGHT_NUM - 1);
			try_num++;
		} while (bg[w1][h1].move_flg == true && try_num < 100);
		if (try_num > 100)
		{
			return;
		}
		do {
			w2 = GetRand(BG_BLOCK_WIDTH_NUM - 1);
			h2 = GetRand(BG_BLOCK_HEIGHT_NUM - 1);
			try_num++;
		} while (bg[w2][h2].move_flg == true && try_num < 100);
		if (try_num > 100)
		{
			return;
		}

		bg[w1][h1].move_flg = true;
		bg[w1][h1].move_goal = bg[w2][h2].location;
		bg[w1][h1].move_rad = atan2f(bg[w1][h1].move_goal.y - bg[w1][h1].location.y, bg[w1][h1].move_goal.x - bg[w1][h1].location.x);
		bg[w2][h2].move_flg = true;
		bg[w2][h2].move_goal = bg[w1][h1].location;
		bg[w2][h2].move_rad = atan2f(bg[w2][h2].move_goal.y - bg[w2][h2].location.y, bg[w2][h2].move_goal.x - bg[w2][h2].location.x);
	}
	for (int i = 0; i < BG_BLOCK_WIDTH_NUM; i++)
	{
		for (int j = 0; j < BG_BLOCK_HEIGHT_NUM; j++)
		{
			if (bg[i][j].move_flg == true)
			{
				bg[i][j].location.x += bg[i][j].move_speed * cosf(bg[i][j].move_rad);
				bg[i][j].location.y += bg[i][j].move_speed * sinf(bg[i][j].move_rad);
				if (bg[i][j].location.x > bg[i][j].move_goal.x - 5 &&
					bg[i][j].location.x < bg[i][j].move_goal.x + 5 &&
					bg[i][j].location.y > bg[i][j].move_goal.y - 5 &&
					bg[i][j].location.y < bg[i][j].move_goal.y + 5)
				{
					bg[i][j].location = bg[i][j].move_goal;
					bg[i][j].move_flg = false;
					bg[i][j].anim_size = 10;
				}
			}
			else if (bg[i][j].anim_size > 0)
			{
				bg[i][j].anim_size--;
			}
		}
	}
}

int Title::GetRandColor()
{
	int rand = GetRand(99);
	if (rand >= 0 && rand < 85)
	{
		return 0x000000;
	}
	else if (rand >= 85 && rand < 90)
	{
		return 0x440000;
	}
	else if (rand >= 90 && rand < 95)
	{
		return 0x004400;
	}
	else if (rand >= 95&& rand < 100)
	{
		return 0x000044;
	}
	return 0x222222;
}

void Title::DrawPlayer(int _num)const
{
	switch (_num)
	{
		case 0:
			//帽子　中央
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 20, player_location.y + 20, player_location.x + 40, player_location.y + 20, player_color, true);
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 20, player_location.y + 20, player_location.x + 40, player_location.y + 20, 0x000000, false);
			//帽子　右側	  									   					  						  						  		 
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 40, player_location.y + 20, player_location.x + 52, player_location.y + 15, player_color, true);
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 40, player_location.y + 20, player_location.x + 52, player_location.y + 15, 0x000000, false);
			//帽子　左側	  									   					  						  						  		 
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 8, player_location.y + 15, player_location.x + 20, player_location.y + 20, player_color, true);
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 8, player_location.y + 15, player_location.x + 20, player_location.y + 20, 0x000000, false);
			
			//頭
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT) + 76, 23, 15, player_location.x, player_location.y, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT) + 76, 23, 15, player_location.x, player_location.y, 0, 0x000000, false);
										 					  
			//目											  
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) + 6, player_location.y - (PLAYER_HEIGHT) + 76, 6, 7, player_location.x, player_location.y, 0, 0x000000, true);
															  
			//首											  
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT) + 62, 10, 5, player_location.x, player_location.y, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT) + 62, 10, 5, player_location.x, player_location.y, 0, 0x000000, false);
															  
			//胴体					 						  
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT) + 37, 21, 37, player_location.x, player_location.y, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT) + 37, 21, 37, player_location.x, player_location.y, 0, 0x000000, false);
															  
			//バッグ					  					  
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) - 15, player_location.y - (PLAYER_HEIGHT) + 40, 5, 23, player_location.x, player_location.y, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) - 15, player_location.y - (PLAYER_HEIGHT) + 40, 5, 23, player_location.x, player_location.y, 0, 0x000000, false);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) - 15, player_location.y - (PLAYER_HEIGHT) + 40, 3, 15, player_location.x, player_location.y, 0, 0x000000, true);
										 
			//腕							
			ResourceManager::DrawRotaBox(player_location.x + 15, player_location.y + 55, 28, 7, player_location.x + 25, player_location.y + 55, 0 + 180, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x + 15, player_location.y + 55, 28, 7, player_location.x + 25, player_location.y + 55, 0 + 180, 0x000000, false);
										 
			//足							
			ResourceManager::DrawRotaBox(player_location.x + 30, player_location.y + 70, 7, 27, player_location.x + 30, player_location.y + 80, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x + 30, player_location.y + 70, 7, 27, player_location.x + 30, player_location.y + 80, 0, 0x000000, false);
																																							 
			break;
		case 1:
		case 2:
			//帽子　中央
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y+3, player_location.x + 20, player_location.y + 23, player_location.x + 40, player_location.y + 23, player_color, true);
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y+3, player_location.x + 20, player_location.y + 23, player_location.x + 40, player_location.y + 23, 0x000000, false);
			//帽子　右側	  									 					 						  						  		 
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y+3, player_location.x + 40, player_location.y + 23, player_location.x + 52, player_location.y + 18, player_color, true);
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y+3, player_location.x + 40, player_location.y + 23, player_location.x + 52, player_location.y + 18, 0x000000, false);
			//帽子　左側	  									 				  						  						  		 
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y+3, player_location.x + 8, player_location.y + 18, player_location.x + 20, player_location.y + 23, player_color, true);
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y+3, player_location.x + 8, player_location.y + 18, player_location.x + 20, player_location.y + 23, 0x000000, false);
		
			//頭
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+76, 23, 15, player_location.x, player_location.y, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+76, 23, 15, player_location.x, player_location.y, 0, 0x000000, false);

			//目											  
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) + 6, player_location.y - (PLAYER_HEIGHT)+82, 6, 7, player_location.x, player_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) - 6, player_location.y - (PLAYER_HEIGHT)+82, 6, 7, player_location.x, player_location.y, 0, 0x000000, true);

			//首											  
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+62, 10, 5, player_location.x, player_location.y, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+62, 10, 5, player_location.x, player_location.y, 0, 0x000000, false);

			//腕							
			ResourceManager::DrawRotaBox(player_location.x + 15, player_location.y + 55, 28, 7, player_location.x + 25, player_location.y + 55, 0 + 180, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x + 15, player_location.y + 55, 28, 7, player_location.x + 25, player_location.y + 55, 0 + 180, 0x000000, false);
			ResourceManager::DrawRotaBox(player_location.x + 25, player_location.y + 55, 28, 7, player_location.x + 35, player_location.y + 55, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x + 25, player_location.y + 55, 28, 7, player_location.x + 35, player_location.y + 55, 0, 0x000000, false);

			//胴体					 						  
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+37, 21, 37, player_location.x, player_location.y, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+37, 21, 37, player_location.x, player_location.y, 0, 0x000000, false);

	
			//足												 														
			ResourceManager::DrawRotaBox(player_location.x + 35, player_location.y + 70, 7, 27, player_location.x + 35, player_location.y + 80, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x + 35, player_location.y + 70, 7, 27, player_location.x + 35, player_location.y + 80, 0, 0x000000, false);							
			ResourceManager::DrawRotaBox(player_location.x + 25, player_location.y + 70, 7, 27, player_location.x + 25, player_location.y + 80, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x + 25, player_location.y + 70, 7, 27, player_location.x + 25, player_location.y + 80, 0, 0x000000, false);

			break;
		case 3:
			//帽子　中央
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 20, player_location.y + 20, player_location.x + 40, player_location.y + 20, player_color, true);
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 20, player_location.y + 20, player_location.x + 40, player_location.y + 20, 0x000000, false);
			//帽子　右側	  									   					  						  						  		 
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 40, player_location.y + 20, player_location.x + 52, player_location.y + 15, player_color, true);
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 40, player_location.y + 20, player_location.x + 52, player_location.y + 15, 0x000000, false);
			//帽子　左側	  									   					  						  						  		 
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 8, player_location.y + 15, player_location.x + 20, player_location.y + 20, player_color, true);
			DrawTriangleAA(player_location.x + (PLAYER_WIDTH / 2), player_location.y, player_location.x + 8, player_location.y + 15, player_location.x + 20, player_location.y + 20, 0x000000, false);
			
			//頭
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+76, 23, 15, player_location.x, player_location.y, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+76, 23, 15, player_location.x, player_location.y, 0, 0x000000, false);

			//目
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) - 6, player_location.y - (PLAYER_HEIGHT)+76, 6, 7, player_location.x, player_location.y, 0, 0x000000, true);

			//首
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+62, 10, 5, player_location.x, player_location.y, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+62, 10, 5, player_location.x, player_location.y, 0, 0x000000, false);

			//胴体																					
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+37, 21, 37, player_location.x, player_location.y, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2), player_location.y - (PLAYER_HEIGHT)+37, 21, 37, player_location.x, player_location.y, 0, 0x000000, false);

			//バッグ
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) + 15, player_location.y - (PLAYER_HEIGHT)+40, 5, 23, player_location.x, player_location.y, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) + 15, player_location.y - (PLAYER_HEIGHT)+40, 5, 23, player_location.x, player_location.y, 0, 0x000000, false);
			ResourceManager::DrawRotaBox(player_location.x - (PLAYER_WIDTH / 2) + 15, player_location.y - (PLAYER_HEIGHT)+40, 3, 15, player_location.x, player_location.y, 0, 0x000000, true);

			//腕
			ResourceManager::DrawRotaBox(player_location.x + 25, player_location.y + 55, 28, 7, player_location.x + 35, player_location.y + 55, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x + 25, player_location.y + 55, 28, 7, player_location.x + 35, player_location.y + 55, 0, 0x000000, false);

			//足												 														
			ResourceManager::DrawRotaBox(player_location.x + 30, player_location.y + 70, 7, 27, player_location.x + 30, player_location.y + 80, 0, player_color, true);
			ResourceManager::DrawRotaBox(player_location.x + 30, player_location.y + 70, 7, 27, player_location.x + 30, player_location.y + 80, 0, 0x000000, false);
			break;
		default:
			break;
	}
}

Vector2D Title::RotationLocation(Vector2D BaseLoc, Vector2D Loc, float r) const
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
