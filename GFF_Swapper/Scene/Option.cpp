#include <dxlib.h>
#include "Option.h"
#include "GameMain.h"
#include "../Utility/PadInput.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "../Utility/ResourceManager.h"
#include "../Utility/DebugInfomation.h"
#include "Title.h"

Option::Option(AbstractScene* _old_scene):
	frame(0),
	item_location{0},
	current_item(-1),	//何も選択していない事を示す-1
	cursor_num(0),
	right_box_location{0},
	right_box_size{0},
	current_bar(-1),	//何も選択していない事を示す-1
	v_cursor_num(0),
	volume_bar_location{0},
	volume_control_bar{0},
	stick_loc(0),
	stick_angle(0.f),
	stick_radian(0.f),
	move_stick(false),
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

	//オプション画面のBGM再生
	ResourceManager::StartSound(ResourceManager::SetSound("Resource/Sounds/BGM/Title.wav", false));
}

void Option::Finalize()
{

}

AbstractScene* Option::Update()
{
	frame++;
	//十字キー上下で選択されている要素を切り替え 他の要素が既に選択されていたら動かせないように
	if (current_item == -1)
	{
		//カーソル上移動
		if (PadInput::OnButton(XINPUT_BUTTON_DPAD_UP))
		{
			if (--cursor_num < 0)
			{
				cursor_num = ITEMS_NUM - 1;
			}
			ResourceManager::StartSound(cursor_se);
		}

		//カーソル下移動
		if (PadInput::OnButton(XINPUT_BUTTON_DPAD_DOWN))
		{
			if (++cursor_num > ITEMS_NUM - 1)
			{
				cursor_num = 0;
			}
			ResourceManager::StartSound(cursor_se);
		}

		//Bボタンが押された時に選択されている要素に切り替え
		if (PadInput::OnRelease(XINPUT_BUTTON_B))
		{
			current_item = cursor_num;
		}
	}

	//Aボタンが押された時に選択されている要素を解除(音量バー選択から一気に2段階戻るのは直すべきか)
	if (PadInput::OnRelease(XINPUT_BUTTON_A))
	{
		if(current_bar !=-1)current_bar = -1;
		else current_item = -1;
	}

	//選択されている要素に応じて処理を変える
	switch (current_item)
	{
	case (int)Items::VOLUME_SETTING:
		UpdateVolumeSetting();
		break;
	case (int)Items::FRAME_RATE:
		break;
	case (int)Items::SE_VOLUME:
		break;
	case (int)Items::BACK:
		//シーン遷移するか確認
		return CheckReturnOldScene();
		break;
	default:
		break;
	}

	//背景の更新(ひとまず動かさない方向で)
	//BackGroundUpdate();

	return this;
}


void Option::Draw() const
{
	DrawStringF(0, 0, "option", 0x00ff00, true);

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
			DrawFormatStringF(item_location[i].x,
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
			DrawFormatStringF(item_location[i].x,
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
	case (int)Items::FRAME_RATE:
		break;
	case (int)Items::SE_VOLUME:
		break;
	case (int)Items::BACK:
		break;
	default:
		//半透明に設定されていたら元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	}
//////右側の設定描画終わり//////
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

void Option::UpdateVolumeSetting()
{
	//十字キー上下で選択されている要素を切り替え 他の要素が既に選択されていたら動かせないように
	if (current_bar == -1)
	{
		//カーソル上移動
		if (PadInput::OnButton(XINPUT_BUTTON_DPAD_UP))
		{
			if (--v_cursor_num < 0)
			{
				v_cursor_num = 2;
			}
			ResourceManager::StartSound(cursor_se);
		}

		//カーソル下移動
		if (PadInput::OnButton(XINPUT_BUTTON_DPAD_DOWN))
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
		current_bar = -1;
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
		//選択されている要素がないなら、音量バーは半透明にする
		if (current_bar != i)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
		}
		//スティック描画用変数定義
		Vector2D stick_inclination;

		//音量バーの上に文字描画
		DrawFormatStringF(volume_bar_location[i].x + 127.5f - (GetDrawStringWidth(VolumeString[i], strlen("今日もいい天気だ")) / 2),
			volume_bar_location[i].y - 40,
			0xffffff, "%s", VolumeString[i]);
		
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