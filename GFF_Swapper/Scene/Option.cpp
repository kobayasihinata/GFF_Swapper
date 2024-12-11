#include <dxlib.h>
#include "Option.h"
#include "GameMain.h"
#include "../Utility/PadInput.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "../Utility/ResourceManager.h"
#include "../Utility/DebugInfomation.h"
#include "Title.h"

Option::Option(AbstractScene* _old_scene) :
	frame(0),
	item_location{ 0 },
	current_item(-1),	//何も選択していない事を示す-1
	cursor_num(0),
	right_box_location{ 0 },
	right_box_size{ 0 },
	current_bar(-1),	//何も選択していない事を示す-1
	v_cursor_num(0),
	volume_bar_location{ 0 },
	volume_control_bar{ 0 },
	stick_loc(0),
	stick_angle(0.f),
	stick_radian(0.f),
	move_stick(false),
	action_box_location{ 0 },
	now_input(0),
	action_num_x(0),
	action_num_y(0),
	current_action_x(-1),//何も選択していない事を示す-1
	current_action_y(-1),//何も選択していない事を示す-1
	wait_timer(0),
	warning_flg(false),
	swap_action(-1),	//何も選択していない事を示す-1
	press_flg(false),
	anim_flg(false),
	back_cursor(0),
	cursor_se(0)
{
	this->old_scene = _old_scene;
}

Option::~Option()
{

}

void Option::Initialize()
{
	__super::Initialize();

	//オプション要素描画位置を設定
	for (int i = 0; i < ITEMS_NUM; i++)
	{
		item_location[i].x = 20;
		item_location[i].y = ITEMS_SHIFT_Y;
		//高さを計算する
		for (int j = i - 1; j >= 0; j--)
		{
			item_location[i].y += ItemsSize[i].y;
			item_location[i].y += ITEMS_SPACE_Y;
		}
	}

	//右側の箱の位置を計算
	right_box_location.x = item_location[0].x + ItemsSize[0].x + RIGHT_BOX_SPACE;
	right_box_location.y = RIGHT_BOX_SPACE;

	//右側の箱の大きさを計算
	right_box_size.x = SCREEN_WIDTH - right_box_location.x - RIGHT_BOX_SPACE;
	right_box_size.y = SCREEN_HEIGHT - right_box_location.y - RIGHT_BOX_SPACE;

	//音量調節バーの位置計算&音量取得
	for (int i = 0; i < 3; i++)
	{
		volume_bar_location[i].x = right_box_location.x + (right_box_size.x / 2) - 127.5f;
		volume_bar_location[i].y = 100 + ((right_box_location.y + VOLUME_SETTING_HEIGHT + VOLUME_SETTING_SPACE) * i);
		volume_control_bar[i] = ResourceManager::GetSoundVolume(i);
	}

	//キー設定箱の位置計算
	for (int i = 0; i < PLAYER_INPUT_NUM; i++)
	{
		action_box_location[i].x = right_box_location.x + (right_box_size.x / 2) - KEY_BOX_WIDTH * 1.5f;
		action_box_location[i].y = right_box_location.y + KEY_BOX_SHIFT + (i * KEY_BOX_HEIGHT);
	}
	
	//背景の初期化
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
			bg[i][j].color = GetColor(i,j);
			bg[i][j].move_rad = 0.0f;
			bg[i][j].anim_size = 0;
		}
	}
	bg_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT);

	stick_loc = { right_box_location.x + (right_box_size.x / 2),right_box_location.y + right_box_size.y+75 };

	cursor_se = ResourceManager::SetSound("Resource/Sounds/Player/cursor.wav");

	//全てのBGMを停止
	ResourceManager::StopAllSound();

	//オプション画面のBGM再生
	ResourceManager::StartSound(ResourceManager::SetSound("Resource/Sounds/BGM/Title.wav", false));
}

void Option::Finalize()
{
	//全てのBGMを停止
	ResourceManager::StopAllSound();
	//ユーザー情報格納
	UserData::Initialize();
}

AbstractScene* Option::Update()
{
	//キー情報取得
	DebugInfomation::Add("state", PadInput::GetNowInput());

	frame++;

	//アニメーション用フラグ更新
	if (frame % 20 == 0)anim_flg = !anim_flg;

	//選択されている要素に応じて処理を変える
	switch (current_item)
	{
	case (int)Items::VOLUME_SETTING:
		UpdateVolumeSetting();
		break;
	case (int)Items::KEY_CONFIG:
		UpdateKeyConfig();
		break;
	case (int)Items::BACK:
		return UpdateBack();
		break;
	default:
		//何も選択されていないなら、左側の要素選択ボックスを更新する
		UpdateLeftBox();
		break;
	}

	//背景の更新(ひとまず動かさない方向で)
	//BackGroundUpdate();

	return this;
}

void Option::Draw() const
{
	int old_size = GetFontSize();
	SetFontSize(24);

//////背景描画//////
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, TRUE);

	//動いていないブロックを先に描画
	for (int i = 0; i < BG_BLOCK_WIDTH_NUM; i++)
	{
		for (int j = 0; j < BG_BLOCK_HEIGHT_NUM; j++)
		{
			if (bg[i][j].move_flg == false && bg[i][j].flg == true)
			{
				DrawBoxAA(bg[i][j].location.x - bg[i][j].anim_size,
					bg[i][j].location.y - bg[i][j].anim_size,
					bg[i][j].location.x + bg[i][j].erea.x + bg[i][j].anim_size,
					bg[i][j].location.y + bg[i][j].erea.y + bg[i][j].anim_size, bg[i][j].color, TRUE);
				DrawBoxAA(bg[i][j].location.x - bg[i][j].anim_size,
					bg[i][j].location.y - bg[i][j].anim_size,
					bg[i][j].location.x + bg[i][j].erea.x + bg[i][j].anim_size,
					bg[i][j].location.y + bg[i][j].erea.y + bg[i][j].anim_size, 0x444444, FALSE);
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
					bg[i][j].location.y + bg[i][j].erea.y + bg[i][j].anim_size, bg[i][j].color + 0x111111, TRUE);
				DrawBoxAA(bg[i][j].location.x - bg[i][j].anim_size,
					bg[i][j].location.y - bg[i][j].anim_size,
					bg[i][j].location.x + bg[i][j].erea.x + bg[i][j].anim_size,
					bg[i][j].location.y + bg[i][j].erea.y + bg[i][j].anim_size, 0x555555, FALSE);
			}
		}
	}

	GetDrawScreenGraph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg_handle);	//背景をハンドルに保存
	ClearDrawScreen();													//画面の初期化
	GraphFilter(bg_handle, DX_GRAPH_FILTER_GAUSS, 16, 1400);			//保存した背景にぼかしをかける
	DrawGraph(0, 0, bg_handle, TRUE);									//背景描画
//////背景描画終わり//////


//////左側の設定項目一覧描画//////
	//選択されている要素があるなら、左側の箱は半透明にする
	if (current_item != -1)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
	}
	//オプション要素の描画
	for (int i = 0; i < ITEMS_NUM; i++)
	{

		//選択されている要素なら強調表示
		if (i == current_item)
		{
			//オプション要素の箱描画（白）
			DrawBoxAA(item_location[i].x,
				item_location[i].y,
				item_location[i].x + ItemsSize[i].x,
				item_location[i].y + ItemsSize[i].y, 0xffffff, true);
			//オプション要素の文字描画（黒）
			DrawFormatStringF(item_location[i].x + (ItemsSize[i].x / 2)- (GetDrawStringWidth(ItemString[i], strlen(ItemString[i])) / 2),
				item_location[i].y + (ItemsSize[i].y / 2) - 10, 0x000000, "%s", ItemString[i]);
		}
		//カーソルが合っていない要素なら通常表示
		else
		{
			//オプション要素の箱描画（白、塗りつぶしなし）
			DrawBoxAA(item_location[i].x,
				item_location[i].y,
				item_location[i].x + ItemsSize[i].x,
				item_location[i].y + ItemsSize[i].y, 0xffffff, false);

			//オプション要素の文字描画（白）
			DrawFormatStringF(item_location[i].x + (ItemsSize[i].x / 2) - (GetDrawStringWidth(ItemString[i], strlen(ItemString[i])) / 2),
				item_location[i].y + (ItemsSize[i].y / 2) - 10, 0xffffff, "%s", ItemString[i]);
		}
		//カーソルが合っている要素なら少し強調表示
		if (i == cursor_num)
		{
			//オプション要素の箱描画（赤、塗りつぶしなし）
			DrawBoxAA(item_location[i].x,
				item_location[i].y,
				item_location[i].x + ItemsSize[i].x,
				item_location[i].y + ItemsSize[i].y, 0xff0000, false);
		}
	}

	//半透明に設定されていたら元に戻す
	if (current_item != -1)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
//////左側の設定項目一覧描画終わり//////


//////右側の設定描画//////
	//選択されている要素がないなら、右側の箱は半透明にする
	if (current_item == -1)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
	}

	//右側の箱描画(白、塗りつぶしなし)
	DrawBoxAA(right_box_location.x,
			right_box_location.y,
			right_box_location.x+right_box_size.x,
			right_box_location.y+right_box_size.y, 0xffffff, false);
	//選択されている要素に応じて描画切り替え
	switch (current_item)
	{
		//ボリューム調整
	case (int)Items::VOLUME_SETTING:
		DrawVolumeSetting();
		break;
	case (int)Items::KEY_CONFIG:
		DrawKeyConfig();
		break;
	case (int)Items::BACK:
		DrawBack();
		break;
	default:
		//半透明に設定されていたら元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	}
//////右側の設定描画終わり//////

//////その他//////

	//ユーザーガイド描画
	DrawUserGuide();

	//フォントサイズをもとに戻す
	SetFontSize(old_size);
}

void Option::BackGroundUpdate()
{
	//背景の交換
	if (frame % 120 == 0 || PadInput::OnRelease(XINPUT_BUTTON_A))
	{
		int w1, h1, w2, h2, try_num = 0;
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

void Option::UpdateLeftBox()
{
	//カーソル上移動
	if (PadInput::GetPressedButton(XINPUT_BUTTON_DPAD_UP) || PadInput::GetPressedButton(L_STICK_UP))
		{
			if (--cursor_num < 0)
			{
				cursor_num = ITEMS_NUM - 1;
			}
			ResourceManager::StartSound(cursor_se);
		}

	//カーソル下移動
	else if (PadInput::GetPressedButton(XINPUT_BUTTON_DPAD_DOWN) || PadInput::GetPressedButton(L_STICK_DOWN))
		{
			if (++cursor_num > ITEMS_NUM - 1)
			{
				cursor_num = 0;
			}
			ResourceManager::StartSound(cursor_se);
		}

	//Bボタンが押された時に選択されている要素に切り替え
	if (PadInput::OnButton(XINPUT_BUTTON_B))
		{
			current_item = cursor_num;
		}

	//Aボタンが押された時に前の画面に戻る処理に移動する
	if (PadInput::OnButton(XINPUT_BUTTON_A))
	{
		current_item = (int)Items::BACK;
	}
}

void Option::UpdateVolumeSetting()
{
	//十字キー上下で選択されている要素を切り替え 他の要素が既に選択されていたら動かせないように
	if (current_bar == -1)
	{
		//カーソル上移動
		if (PadInput::GetPressedButton(XINPUT_BUTTON_DPAD_UP)|| PadInput::GetPressedButton(L_STICK_UP))
		{
			if (--v_cursor_num < 0)
			{
				v_cursor_num = 2;
			}
			ResourceManager::StartSound(cursor_se);
		}

		//カーソル下移動
		else if (PadInput::GetPressedButton(XINPUT_BUTTON_DPAD_DOWN) || PadInput::GetPressedButton(L_STICK_DOWN))
		{
			if (++v_cursor_num > 2)
			{
				v_cursor_num = 0;
			}
			ResourceManager::StartSound(cursor_se);
		}

		//Bボタンが押された時に選択されている要素に切り替え
		if (PadInput::OnButton(XINPUT_BUTTON_B))
		{
			current_bar = v_cursor_num;
		}
	}

	//Aボタンが押された時に選択されている要素を解除
	if (PadInput::OnRelease(XINPUT_BUTTON_A))
	{
		//音量調整バーが選択されていたら、選択を解除
		if (current_bar != -1)current_bar = -1;
		//音量調整バーが選択されていなければ、要素選択ボックスに戻る
		else current_item = -1;

	}

	//何かしらのバーが選択されているなら音量更新
	if (current_bar != -1)
	{
		ChangeVolume(current_bar);
	}
}

void Option::ChangeVolume(int _num)
{

	//スティックが一定以上傾いているなら、スティックの角度に応じて音量を変更する

	//スティックが動いているかのフラグをリセット
	move_stick = false;
	//スティックの描画位置を移動
	stick_loc.y = (volume_bar_location[_num].y + VOLUME_BAR_HEIGHT + 75);
	//下に傾けた場合は反応しない
	if (
		(PadInput::TipLStick(STICKL_X) > 0.5f || 
		PadInput::TipLStick(STICKL_X) < -0.5f ||
		PadInput::TipLStick(STICKL_Y) > 0.5f)
		&&
		PadInput::TipLStick(STICKL_Y) > 0.f
		)
	{
		move_stick = true;
		//スティックの角度を求める
		Vector2D stick_inclination = { PadInput::TipLStick(STICKL_X) * 10,PadInput::TipLStick(STICKL_Y) * 10 };
		stick_radian = atan2f(stick_inclination.y, stick_inclination.x);
		stick_angle = (float)(180.f - (stick_radian * 180.f / M_PI));
		DebugInfomation::Add("stick_angle", stick_angle);

		//スティックの角度が40~140なら応じた数値にする
		if (stick_angle > 40.f && stick_angle < 140.f)
		{
			//角度に応じて音量を変更する
			volume_control_bar[_num] = (int)((stick_angle - 40) * 2.55f);
		}
		//角度が40以下なら音量を0にする
		else if (stick_angle < 40.f)
		{
			volume_control_bar[_num] = 0;
		}
		//角度が140以上なら音量を255(最大)にする
		else
		{
			volume_control_bar[_num] = 255;
		}

		//音量を更新する
		ResourceManager::SetSoundVolume(_num,volume_control_bar[_num]);

		//カーソル音の再生を確認するために間隔を置く
		if (frame % 4 == 0)
		{
			ResourceManager::StartSound(cursor_se);
		}
	}
	//カーソル音の再生を確認するために間隔を置く
	if (frame % 3 == 0)
	{
		//左右キーで音量を調整
		if (PadInput::OnPressed(XINPUT_BUTTON_DPAD_RIGHT))
		{
			if (volume_control_bar[_num] < 255)volume_control_bar[_num]++;
			ResourceManager::SetSoundVolume(_num,volume_control_bar[_num]);
			ResourceManager::StartSound(cursor_se);
		}
		if (PadInput::OnPressed(XINPUT_BUTTON_DPAD_LEFT))
		{
			if (volume_control_bar[_num] > 0)volume_control_bar[_num]--;
			ResourceManager::SetSoundVolume(_num,volume_control_bar[_num]);
			ResourceManager::StartSound(cursor_se);
		}
	}
	//バーの大きさを再取得
	volume_control_bar[_num] = ResourceManager::GetSoundVolume(_num);
}

void Option::DrawVolumeSetting()const
{
	SetFontSize(30);
	for (int i = 0; i < 3; i++)
	{
		//音量バーの上に文字描画
		DrawFormatStringF(volume_bar_location[i].x + 127.5f - (GetDrawStringWidth(VolumeString[i], strlen(VolumeString[i])) / 2),
			volume_bar_location[i].y - 40,
			0xffffff, "%s", VolumeString[i]);

		//選択されている要素でないなら、音量バーは半透明にする
		if (current_bar != i)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
		}
		//スティック描画用変数定義
		Vector2D stick_inclination;
		
		//音量バー
		DrawBoxAA(volume_bar_location[i].x,
			volume_bar_location[i].y,
			volume_bar_location[i].x  + volume_control_bar[i],
			volume_bar_location[i].y + VOLUME_BAR_HEIGHT, 0x00ff00, true);

		//半透明に設定されていたら元に戻す
		if (current_bar != i)
		{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		//選択されている外枠の下にスティックを描画する
		else
		{
			//スティックが動いているときしか描画しない線
			if (move_stick)
			{
				//スティックとバーを繋ぐ線描画
				DrawLineAA(stick_loc.x,
					stick_loc.y,
					stick_loc.x + (cosf(stick_radian) * 200),
					stick_loc.y - (sinf(stick_radian) * 200), 0xff0000);
			}

			//スティック描画
			stick_inclination = { PadInput::TipLStick(STICKL_X) * 10,PadInput::TipLStick(STICKL_Y) * 10 };
			DrawCircleAA(stick_loc.x, stick_loc.y, 20, 20, 0xaaaaaa, false);
			DrawCircleAA(stick_loc.x + stick_inclination.x, stick_loc.y - stick_inclination.y, 15, 20, 0xaaaaaa, true);
		}

		//カーソルが合っているボックスを赤色で描画
		if (i == v_cursor_num)
		{
			//音量バー外枠　赤
			DrawBoxAA(volume_bar_location[i].x,
				volume_bar_location[i].y,
				volume_bar_location[i].x + 255,
				volume_bar_location[i].y + VOLUME_BAR_HEIGHT, 0xff0000, false);
			//矢印
			DrawTriangleAA(volume_bar_location[i].x - 40,
				volume_bar_location[i].y + 5,
				volume_bar_location[i].x - 10,
				volume_bar_location[i].y + (VOLUME_BAR_HEIGHT / 2),
				volume_bar_location[i].x - 40,
				volume_bar_location[i].y + VOLUME_BAR_HEIGHT - 5, 0xff0000, true);
		}
		//カーソルが合っていないボックスを白色で描画
		else
		{
			//音量バー外枠　白
			DrawBoxAA(volume_bar_location[i].x,
				volume_bar_location[i].y,
				volume_bar_location[i].x + 255,
				volume_bar_location[i].y + VOLUME_BAR_HEIGHT, 0xffffff, false);
		}
	}
}

void Option::UpdateKeyConfig()
{
	
	//どのキーも押していない事が検知されたらフラグをおろす
	if (PadInput::GetNowInput() == -1)press_flg = false;

	//キーの割り当て処理をしていないなら更新
	if (current_action_y == -1 && !press_flg)
	{
		//カーソル上移動
		if (PadInput::GetPressedButton(XINPUT_BUTTON_DPAD_UP) || PadInput::GetPressedButton(L_STICK_UP))
		{
			if (--action_num_y < 0)
			{
				action_num_y = PLAYER_INPUT_NUM - 1;
			}
			ResourceManager::StartSound(cursor_se);
		}

		//カーソル下移動
		else if (PadInput::GetPressedButton(XINPUT_BUTTON_DPAD_DOWN) || PadInput::GetPressedButton(L_STICK_DOWN))
		{
			if (++action_num_y > PLAYER_INPUT_NUM - 1)
			{
				action_num_y = 0;
			}
			ResourceManager::StartSound(cursor_se);
		}

		//カーソル右移動
		else if (PadInput::GetPressedButton(XINPUT_BUTTON_DPAD_RIGHT) || PadInput::GetPressedButton(L_STICK_RIGHT))
		{
			if (++action_num_x > 1)
			{
				action_num_x = 0;
			}
			ResourceManager::StartSound(cursor_se);
		}

		//カーソル左移動
		else if (PadInput::GetPressedButton(XINPUT_BUTTON_DPAD_LEFT) || PadInput::GetPressedButton(L_STICK_LEFT))
		{
			if (--action_num_x < 0)
			{
				action_num_x = 1;
			}
			ResourceManager::StartSound(cursor_se);
		}

		//Bボタンが押された時に選択されている要素に切り替え
		if (PadInput::OnButton(XINPUT_BUTTON_B))
		{
			current_action_y = action_num_y;
			current_action_x = action_num_x;
			press_flg = true;
		}

		//Aボタンが押された時に選択されている要素を解除
		if (PadInput::OnButton(XINPUT_BUTTON_A))
		{
			current_item = -1;
			press_flg = true;
			warning_flg = false;
		}

		//STARTボタンが押された時にキー割り当てをリセット
		if (PadInput::OnButton(XINPUT_BUTTON_START))
		{
			UserData::ResetKeyConfig();
		}
	}
	//キーの割り当て処理
	else if(!press_flg)
	{
		//警告表示中なら、その処理だけして後の処理は行わない
		if (warning_flg)
		{
			//キーの割り当てを交換
			if (PadInput::OnButton(XINPUT_BUTTON_B))
			{
				int t;
				t = UserData::player_key[current_action_y][current_action_x];
				UserData::player_key[current_action_y][current_action_x] = UserData::player_key[swap_action][current_action_x];
				UserData::player_key[swap_action][current_action_x] = t;

				//フラグをおろす
				warning_flg = false;
				//キーの選択をやめる
				current_action_y = -1;
				press_flg = true;
			}

			//キーの選択をやめる
			if (PadInput::OnButton(XINPUT_BUTTON_A))
			{
				//フラグをおろす
				warning_flg = false;
				//キーの選択をやめる
				current_action_y = -1;
				press_flg = true;
			}

			return;
		}
		//現在押されているキーを格納
		now_input = PadInput::GetNowInput();

		//選択されている時にBACK以外のボタンが押されたら、キーの割り当てを更新する
		if (now_input != -1 && now_input != XINPUT_BUTTON_BACK)
		{
			warning_flg = false;
			//STARTが押されたら空白を表す10を割り振り、警告処理スキップ
			if (now_input == XINPUT_BUTTON_START)now_input = 10;
			//割り当てられているキーを捜索して、割り当てようとしているキーがすでにあるなら、警告を出す
			else
			{
				for (int i = 0; i < PLAYER_INPUT_NUM; i++)
				{
					//同じキーを発見したら、フラグを立て、位置を格納(選択されている要素はスキップ)
					if (UserData::player_key[i][current_action_x] == now_input && current_action_y != i)
					{
						swap_action = i;
						warning_flg = true;
						//5フレーム入力を受け付けない
						wait_timer = 5;
						break;
					}
				}
			}

			//割り当てられていないキーなら
			if (!warning_flg)
			{
				//キーの割り当てを更新する
				UserData::player_key[current_action_y][current_action_x] = now_input;
				//割り当て処理をやめる
				current_action_y = -1;
				//5フレーム入力を受け付けない
				wait_timer = 5;
			}
		}
		//BACKかSTARTが押されたら割り当て処理をやめる
		else if (now_input != -1)
		{
			current_action_y = -1;
			//5フレーム入力を受け付けない
			wait_timer = 5;
		}
	}
}

void Option::DrawKeyConfig()const
{
	SetFontSize(24);

	//現在のキー割り当て描画

	//プレイヤーの入力の数だけ箱を描画する
	for (int i = 0; i < PLAYER_INPUT_NUM; i++)
	{
		//操作一覧を格納する箱(白、塗りつぶしあり)
		DrawBox(action_box_location[i].x,
			action_box_location[i].y,
			action_box_location[i].x + KEY_BOX_WIDTH,
			action_box_location[i].y + KEY_BOX_HEIGHT,
			0xffffff, true);

		//操作一覧を格納する箱の外枠(黒、塗りつぶしなし)
		DrawBox(action_box_location[i].x,
			action_box_location[i].y,
			action_box_location[i].x + KEY_BOX_WIDTH,
			action_box_location[i].y + KEY_BOX_HEIGHT,
			0x000000, false);

		//操作一覧を文字で描画
		DrawFormatString(action_box_location[i].x + 10,
			action_box_location[i].y + 10,
			0x000000, "%s", PlayerAction[i]);

		//割り当てられたキーを格納する用(白 塗りつぶしなし)
		DrawBox(action_box_location[i].x + KEY_BOX_WIDTH,
			action_box_location[i].y,
			action_box_location[i].x + (KEY_BOX_WIDTH * 2), 
			action_box_location[i].y + KEY_BOX_HEIGHT,
			0xffffff, false);

		//一つ目のキーを描画する
		DrawGraph(action_box_location[i].x + 10 + KEY_BOX_WIDTH,
			action_box_location[i].y + 5, 
			ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], UserData::player_key[i][0]), TRUE);
		DrawFormatString(action_box_location[i].x + 50 + KEY_BOX_WIDTH,
			action_box_location[i].y + 10,
			0xffffff, "%s", KeyString[UserData::player_key[i][0]]);

		//割り当てられたキーを格納する用(白 塗りつぶしなし)
		DrawBox(action_box_location[i].x + (KEY_BOX_WIDTH*2),
			action_box_location[i].y,
			action_box_location[i].x + (KEY_BOX_WIDTH * 3),
			action_box_location[i].y + KEY_BOX_HEIGHT,
			0xffffff, false);

		//二つ目のキーを描画する
		DrawGraph(action_box_location[i].x + 10 + KEY_BOX_WIDTH+ KEY_BOX_WIDTH,
			action_box_location[i].y + 5,
			ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], UserData::player_key[i][1]), TRUE);
		DrawFormatString(action_box_location[i].x + 50 + KEY_BOX_WIDTH+ KEY_BOX_WIDTH,
			action_box_location[i].y + 10,
			0xffffff, "%s", KeyString[UserData::player_key[i][1]]);

		//カーソルを描画する
		if (i == action_num_y)
		{
			//操作一覧を格納する箱の外枠(赤、塗りつぶしなし)
			DrawBoxAA(action_box_location[i].x + (KEY_BOX_WIDTH*(action_num_x+1)),
				action_box_location[i].y,
				action_box_location[i].x + (KEY_BOX_WIDTH * (action_num_x + 2)),
				action_box_location[i].y + KEY_BOX_HEIGHT,
				0xff0000, false);
			//操作一覧を格納する箱の外枠(白、塗りつぶしあり)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
			DrawBoxAA(action_box_location[i].x + (KEY_BOX_WIDTH * (action_num_x + 1)),
				action_box_location[i].y,
				action_box_location[i].x + (KEY_BOX_WIDTH * (action_num_x + 2)),
				action_box_location[i].y + KEY_BOX_HEIGHT,
				0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}
	//キー割り当て処理中なら背景を暗転＆説明文を描画
	if (current_action_y != -1)
	{
		//暗転
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		//説明文(警告が出たら隠す)
		if (!warning_flg)
		{
			DrawString(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, "割り当てたいキーを押してください", 0xffffff);
			DrawGraph(SCREEN_WIDTH / 2 - 90, 
				SCREEN_HEIGHT / 2 + 20,
				ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], XINPUT_BUTTON_BACK), TRUE);
			DrawString(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 30, " = 取り消し", 0xffffff);
			DrawGraph(SCREEN_WIDTH / 2 - 90,
				SCREEN_HEIGHT / 2 + 50,
				ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], XINPUT_BUTTON_START), TRUE);
			DrawString(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 60, " = 割り当て解除", 0xffffff);
		}
	}
	//警告表示
	if (warning_flg)
	{
		//警告表示箱（黒、塗りつぶしあり）
		DrawBox((SCREEN_WIDTH / 2) - 250,
			(SCREEN_HEIGHT / 2) - 50,
			(SCREEN_WIDTH / 2) + 250,
			(SCREEN_HEIGHT / 2) + 50, 0x000000, true);

		//警告表示箱（白、塗りつぶしなし）
		DrawBox((SCREEN_WIDTH / 2) - 250,
			(SCREEN_HEIGHT / 2) - 50,
			(SCREEN_WIDTH / 2) + 250,
			(SCREEN_HEIGHT / 2) + 50, 0xffffff, false);

		//警告表示
		DrawString((SCREEN_WIDTH / 2) - (GetDrawStringWidth("---既に割り当てられているキーです！---", strlen("---既に割り当てられているキーです！---")) / 2),
			(SCREEN_HEIGHT / 2) - 40, "---既に割り当てられているキーです！---", 0xffffff);

		//該当アクション表示
		DrawFormatString((SCREEN_WIDTH / 2) - (GetDrawStringWidth(PlayerAction[swap_action], strlen(PlayerAction[swap_action])) / 2),
			(SCREEN_HEIGHT / 2) - 10, 0xffffff, "%s", PlayerAction[swap_action]);

		//選択肢表示
		DrawGraph((SCREEN_WIDTH / 2) - 250,
			(SCREEN_HEIGHT / 2)+10,
			ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], XINPUT_BUTTON_A), TRUE);
		DrawGraph((SCREEN_WIDTH / 2) - 55,
			(SCREEN_HEIGHT / 2)+10,
			ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], XINPUT_BUTTON_B), TRUE);
		DrawString((SCREEN_WIDTH / 2)- 240,
			(SCREEN_HEIGHT / 2) +20, "   = やめる       = 交換して割り当てる", 0xffffff);
	}
}

AbstractScene* Option::UpdateBack()
{
	//カーソル上移動
	if (PadInput::OnButton(XINPUT_BUTTON_DPAD_UP) || PadInput::OnButton(L_STICK_UP))
		{
			if (--back_cursor < 0)
			{
				back_cursor = 2;
			}
			ResourceManager::StartSound(cursor_se);
		}

	//カーソル下移動
	if (PadInput::OnButton(XINPUT_BUTTON_DPAD_DOWN) || PadInput::OnButton(L_STICK_DOWN))
		{
			if (++back_cursor > 2)
			{
				back_cursor = 0;
			}
			ResourceManager::StartSound(cursor_se);
		}

	//Aボタンが押された時に選択されている要素を解除
	if (PadInput::OnButton(XINPUT_BUTTON_A))
	{
		current_item = -1;
	}

	if (PadInput::OnButton(XINPUT_BUTTON_B))
	{
		switch (back_cursor)
		{
			//保存して終了
		case 0:
			//ファイルを更新してから終了
			UserData::UpdateUserData();
			return CheckReturnOldScene();
			break;

			//保存せず終了
		case 1:
			//保存する前のデータを読み込んで、ユーザーデータを変更前の物にしてから終了
			UserData::LoadUserData();
			//変更後のステージで音量が元に戻らないので、リセットした後の音量にする
			ResourceManager::SetSoundVolume(0, UserData::volume[0]);
			ResourceManager::SetSoundVolume(1, UserData::volume[1]);
			ResourceManager::SetSoundVolume(2, UserData::volume[2]);
			return CheckReturnOldScene();
			break;

			//キャンセル
		case 2:
			current_item = -1;
			break;
		default:
			break;
		}
	}
	return this;
}

void Option::DrawBack()const
{
	//後ろの描画を暗転させる
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	SetFontSize(40);

	//描画文字
	DrawString((SCREEN_WIDTH / 2) - (GetDrawStringWidth("変更を保存しますか？", strlen("変更を保存しますか？")) / 2), (SCREEN_HEIGHT / 2) - 70, "変更を保存しますか？", 0xffffff, true);

	//選択要素描画
	SetFontSize(24);
	DrawString((SCREEN_WIDTH / 2) - 150, (SCREEN_HEIGHT / 2), "保存して終了", 0xffffff);
	DrawString((SCREEN_WIDTH / 2) - 150, (SCREEN_HEIGHT / 2) + 24, "保存せず終了", 0xffffff);
	DrawString((SCREEN_WIDTH / 2) - 150, (SCREEN_HEIGHT / 2) + 48, "キャンセル", 0xffffff);

	//カーソル描画
	DrawTriangle((SCREEN_WIDTH / 2) - 190,
		(SCREEN_HEIGHT / 2) + (back_cursor * 24),
		(SCREEN_WIDTH / 2) - 160,
		(SCREEN_HEIGHT / 2) + 6 + (back_cursor * 24),
		(SCREEN_WIDTH / 2) - 190,
		(SCREEN_HEIGHT / 2) + 12 + (back_cursor * 24), 0xffffff, true);
}

AbstractScene* Option::CheckReturnOldScene()
{
	//ひとつ前のシーンがないならタイトル画面に戻る
	if (GetOldScene() == nullptr)
	{
		return new Title();
	}
	else
	{
		//ひとつ前のシーンに戻る
		return GetOldScene();
	}
}

int Option::GetColor(int i, int j)
{
	//横幅の数値に応じて赤緑青灰の順で返す

	switch ((i % 4 + j % 4) % 4)
	{
	case 0:
		return 0x110000;
	case 1:
		return 0x001100;
	case 2:
		return 0x000011;
	case 3:
		return 0x000000;
	default:
		return 0x000000;
	}
	return 0x000000;
}

void Option::DrawUserGuide()const
{
	SetFontSize(24);
	//キー割り当ての表示がない時は描画
	if (current_action_y == -1)
	{
		//上下とBボタンで何か選択する事を描画
		DrawGraph(right_box_location.x,
			right_box_location.y + right_box_size.y - 40,
			ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], XINPUT_BUTTON_DPAD_UP), TRUE);
		DrawGraph(right_box_location.x + 40,
			right_box_location.y + right_box_size.y - 40,
			ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], XINPUT_BUTTON_DPAD_DOWN), TRUE);
		DrawGraph(right_box_location.x + 80,
			right_box_location.y + right_box_size.y - 40,
			ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], L_STICK_UP), TRUE);
		DrawGraph(right_box_location.x + 120,
			right_box_location.y + right_box_size.y - 40,
			ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], L_STICK_DOWN), TRUE);
		DrawStringF(right_box_location.x + 170,
			right_box_location.y + right_box_size.y - 30,
			"＆", 0xffffff, true);
		DrawGraph(right_box_location.x + 200,
			right_box_location.y + right_box_size.y - 40,
			ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], XINPUT_BUTTON_B), TRUE);
		DrawStringF(right_box_location.x + 240,
			right_box_location.y + right_box_size.y - 30,
			" = 選択", 0xffffff, true);
		//何か選択しているときの表示
		if (current_item != -1)
		{
			//Aボタンで選択解除
			DrawGraph(right_box_location.x + 350,
				right_box_location.y + right_box_size.y - 40,
				ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], XINPUT_BUTTON_A), TRUE);
			DrawStringF(right_box_location.x + 390,
				right_box_location.y + right_box_size.y - 30,
				" = 戻る", 0xffffff, true);

			//音量調整中なら左スティックと十字横で調整出来る事を教える
			if (current_bar != -1)
			{
				//Aボタンで選択解除
				DrawGraph(right_box_location.x + 680,
					right_box_location.y + 90 + (current_bar * 210), 
					ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], XINPUT_BUTTON_DPAD_LEFT), TRUE);
				DrawGraph(right_box_location.x + 720,
					right_box_location.y + 90 + (current_bar * 210),
					ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], XINPUT_BUTTON_DPAD_RIGHT), TRUE);
				DrawGraph(right_box_location.x + 760,
					right_box_location.y + 90 + (current_bar * 210),
					ResourceManager::GetDivGraph(UserData::button_image[0], L_STICK_DOWN), TRUE);
				DrawStringF(right_box_location.x + 800,
					right_box_location.y + 100 + (current_bar * 210),
					" = 調整", 0xffffff, true);
			}
			//キー割り当て画面では更にSTARTで割り当てリセット出来る事を描画
			if (current_item == (int)Items::KEY_CONFIG)
			{
				//Aボタンで選択解除
				DrawGraph(right_box_location.x + 500,
					right_box_location.y + right_box_size.y - 40,
					ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], XINPUT_BUTTON_START), TRUE);
				DrawStringF(right_box_location.x + 540,
					right_box_location.y + right_box_size.y - 30,
					" = リセット", 0xffffff, true);
			}
		}
	}
		
}