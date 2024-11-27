#include <dxlib.h>
#include "Option.h"
#include "GameMain.h"
#include "../Utility/PadInput.h"

#include "../Utility/ResourceManager.h"
#include "Title.h"

Option::Option(AbstractScene* _old_scene):
	current_num((int)Items::MASTER_VOLUME)
{
	this->old_scene = _old_scene;
}

Option::~Option()
{

}

void Option::Initialize()
{
	__super::Initialize();

	frame = 0;

	//オプション要素描画位置を設定
	for (int i = 0; i < ITEMS_NUM; i++)
	{
		item_location[i].x = (SCREEN_WIDTH/2) - (ItemsSize[i].x / 2);
		item_location[i].y = ITEMS_SHIFT_Y;
		//高さを計算する
		for (int j = i - 1; j >= 0; j--)
		{
			item_location[i].y += ItemsSize[i].y;
			item_location[i].y += ITEMS_SPACE_Y;
		}
	}

	volume_control_bar = ResourceManager::GetSoundVolume();
	cursor_se = ResourceManager::SetSound("Resource/Sounds/Player/cursor.wav");
}

void Option::Finalize()
{

}

AbstractScene* Option::Update()
{
	frame++;
	//十字キー上下で選択されている要素を切り替え
	if (PadInput::OnButton(XINPUT_BUTTON_DPAD_UP))
	{
		if (--current_num < 0)
		{
			current_num = ITEMS_NUM -1;
		}
		ResourceManager::StartSound(cursor_se);
	}
	if (PadInput::OnButton(XINPUT_BUTTON_DPAD_DOWN))
	{
		if (++current_num > ITEMS_NUM - 1)
		{
			current_num = 0;
		}
		ResourceManager::StartSound(cursor_se);
	}

	//選択されている要素に応じて処理を変える
	switch (current_num)
	{
	case (int)Items::MASTER_VOLUME:
		ChangeMasterVolume();
		break;
	case (int)Items::BGM_VOLUME:
		break;
	case (int)Items::SE_VOLUME:
		break;
	case (int)Items::BACK:
		//シーン遷移するか確認
		return CheckReturnOldScene();
		break;
	}

	return this;
}


void Option::Draw() const
{
	DrawStringF(0, 0, "option", 0x00ff00, true);

	//オプション要素の描画
	for (int i = 0; i < ITEMS_NUM; i++)
	{
		//オプション要素の箱描画
		DrawBoxAA(item_location[i].x,
			item_location[i].y,
			item_location[i].x + ItemsSize[i].x,
			item_location[i].y + ItemsSize[i].y, 0xffffff, false);

		//選択されている要素なら強調表示
		if (i == current_num)
		{
			DrawBoxAA(item_location[i].x,
				item_location[i].y,
				item_location[i].x + ItemsSize[i].x,
				item_location[i].y + ItemsSize[i].y, 0xff0000, false);
		}
	}
	DrawBox(item_location[0].x+25,
		item_location[0].y+20,
		item_location[0].x+25 + volume_control_bar,
		item_location[0].y + ItemsSize[0].y-20, 0x00ff00, true);
}

void Option::ChangeMasterVolume()
{
	//カーソル音の再生を確認するために間隔を置く
	if (frame % 2 == 0)
	{
		//左右キーで音量を調整
		if (PadInput::OnPressed(XINPUT_BUTTON_DPAD_RIGHT))
		{
			if (volume_control_bar < 255)volume_control_bar += 2;
			ResourceManager::SetSoundVolume(volume_control_bar);
			ResourceManager::StartSound(cursor_se);
		}
		if (PadInput::OnPressed(XINPUT_BUTTON_DPAD_LEFT))
		{
			if (volume_control_bar > 0)volume_control_bar -= 2;
			ResourceManager::SetSoundVolume(volume_control_bar);
			ResourceManager::StartSound(cursor_se);
		}
	}
	//バーの大きさを再取得
	volume_control_bar = ResourceManager::GetSoundVolume();
}

AbstractScene* Option::CheckReturnOldScene()
{
	//Bボタンが押されたら遷移
	if (PadInput::OnRelease(XINPUT_BUTTON_B))
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
	//Bボタンが押されなかったら現在のシーンを返す
	return this;
}