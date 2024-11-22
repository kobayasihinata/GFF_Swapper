#include "ResourceManager.h"
#include "common.h"
#define _USE_MATH_DEFINES
#include <math.h>

char* ResourceManager::image_filepath[IMAGE_NUM];
AnimData ResourceManager::div_image_data[DIV_IMAGE_NUM];
char* ResourceManager::sound_filepath[SOUND_NUM];
int ResourceManager::image_data[IMAGE_NUM];
int ResourceManager::sound_data[SOUND_NUM];
int ResourceManager::sound_freq = 50000;
int ResourceManager::se_volume;
int ResourceManager::bgm_volume;
int ResourceManager::master_volume;

int ResourceManager::anim;			
FireAnim ResourceManager::fire_anim[ANIM_BLOCK_NUM];
WoodAnim ResourceManager::wood_anim[ANIM_BLOCK_NUM];
WaterAnim ResourceManager::water_anim[ANIM_BLOCK_NUM];
int ResourceManager::screen = MakeScreen(BOX_WIDTH, BOX_HEIGHT);
int ResourceManager::anim_handle[3] = { MakeScreen(BOX_WIDTH, BOX_HEIGHT),MakeScreen(BOX_WIDTH, BOX_HEIGHT),MakeScreen(BOX_WIDTH, BOX_HEIGHT) };
int ResourceManager::stage_block_handle[3] = { MakeScreen(BOX_WIDTH, BOX_HEIGHT),MakeScreen(BOX_WIDTH, BOX_HEIGHT),MakeScreen(BOX_WIDTH, BOX_HEIGHT) };

void ResourceManager::DeleteResource()
{
	for (int i = 0; image_data[i] != NULL; i++)
	{
		DeleteGraph(image_data[i]);
	}

	for (int i = 0; div_image_data[i].div_image_handle[0] != NULL; i++)
	{
		for (int j = 0; div_image_data[i].div_image_handle[j] != NULL; j++)
		{
			DeleteGraph(div_image_data[i].div_image_handle[j]);
		}
	}

	//for (int i = 0; sound_data[i] != NULL; i++)
	//{
	//	DeleteSoundMem(sound_data[i]);
	//}

	// 音をすべて削除
	InitSoundMem();
}

void ResourceManager::StageAnimInitialize()
{
	anim = 0;
	////エフェクトのアニメーション用初期定義
	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	{
		fire_anim[i].time = GetRand(20);
		wood_anim[i].shift = (float)GetRand(100) / 10;
		wood_anim[i].shift1.x = (float)((BOX_WIDTH / 10) * i);
		wood_anim[i].shift2.x = (float)((BOX_WIDTH / 10) * i);
		wood_anim[i].initial_position = wood_anim[i].shift2;
		wood_anim[i].shift1.y = BOX_HEIGHT;
		wood_anim[i].shift2.y = (float)(GetRand(190) / 10);
		water_anim[i].shift = (float)GetRand(9)/10;
		water_anim[i].shift1.x = (float)(GetRand(8) * (BOX_WIDTH / 10));
		water_anim[i].shift1.y = (float)(GetRand(8) * (BOX_HEIGHT / 10) - 15);
		water_anim[i].initial_position = water_anim[i].shift1;
		water_anim[i].erea.x = BOX_WIDTH /3;
		water_anim[i].erea.y = BOX_HEIGHT /3;
	}

	//ハンドルリセット
	for (int i = 0; i < 3; i++)
	{
		anim_handle[i] = MakeScreen(BOX_WIDTH, BOX_HEIGHT, TRUE);
		stage_block_handle[i] = MakeScreen(BOX_WIDTH, BOX_HEIGHT, TRUE);
	}
}

void ResourceManager::StageAnimUpdate()
{
	//アニメーション用変数
	if (++anim > 300)
	{
		anim = 0;
	}

	//分割画像のアニメーション更新
	for (int i = 0; div_image_data[i].div_image_handle[0] != NULL; i++)
	{
		//アニメーションがない分割画像なら(マップチップ等)アニメーション処理を飛ばす
		if (div_image_data[i].anim_speed <= 0)continue;
		//画像ごとの周期に達したら次の画像に切り替え
		if (anim % div_image_data[i].anim_speed == 0)
		{
			//画像の枚数をオーバーしたら最初の画像へ
			if (++div_image_data[i].now_image >= div_image_data[i].div_image_num)
			{
				div_image_data[i].now_image = 0;
			}
		}
	}

	//画像を使わない時のアニメーション更新

	////炎エフェクト用
	//for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	//{
	//	if (fire_anim[i].time < 0)
	//	{
	//		fire_anim[i].shift.x = 0;
	//		fire_anim[i].shift.y = BOX_HEIGHT;
	//		fire_anim[i].erea.x = 4;
	//		fire_anim[i].erea.y = 4;
	//		fire_anim[i].shift.x += 4 * GetRand(10);
	//		fire_anim[i].time = 30 + GetRand(30);
	//		fire_anim[i].angle = (float)(GetRand(30) / 10);
	//	}
	//	else
	//	{
	//		fire_anim[i].shift.x += 0.05f + (fire_anim[i].angle - 1) / 2;
	//		fire_anim[i].shift.y -= 1;
	//		fire_anim[i].time--;
	//		fire_anim[i].erea.x -= 0.05f;
	//		fire_anim[i].erea.y -= 0.05f;
	//	}
	//}

	////木エフェクト用
	//for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	//{
	//	if (anim < 30)
	//	{
	//		wood_anim[i].shift2.x += (wood_anim[i].shift / 50);
	//	}
	//	else
	//	{
	//		wood_anim[i].shift2.x -= (wood_anim[i].shift / 50);
	//	}
	//	if (anim >= 60)
	//	{
	//		wood_anim[i].shift2.x = wood_anim[i].initial_position.x;
	//	}
	//}

	////水エフェクト更新
	//for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	//{
	//	if (anim <= 30)
	//	{
	//		water_anim[i].shift1.y += water_anim[i].shift;
	//	}
	//	else
	//	{
	//		water_anim[i].shift1.y -= water_anim[i].shift;
	//	}
	//	if (anim >= 60)
	//	{
	//		water_anim[i].shift1.y = water_anim[i].initial_position.y;
	//	}
	//}

	////アニメーションをハンドルに保存する
	//SaveAnimHandle();
}

void ResourceManager::SaveAnimHandle()
{
	//炎
	SetDrawScreen(anim_handle[0]);
	ClearDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 130);
	DrawBoxAA(0, 0, BOX_WIDTH, BOX_HEIGHT, 0xff0000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	{
		if (i % 3 == 0)
		{
			DrawBoxAA(fire_anim[i].shift.x,
					  fire_anim[i].shift.y,
					  fire_anim[i].shift.x + fire_anim[i].erea.x,
					  fire_anim[i].shift.y + fire_anim[i].erea.y, 0xff9900, true);
		}
		else
		{
			DrawBoxAA(fire_anim[i].shift.x,
					  fire_anim[i].shift.y,
					  fire_anim[i].shift.x + fire_anim[i].erea.x,
					  fire_anim[i].shift.y + fire_anim[i].erea.y, 0xff0000, true);
		}
	}

	//草
	SetDrawScreen(anim_handle[1]);
	ClearDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBoxAA(0, 0, BOX_WIDTH, BOX_HEIGHT, 0x00ff00, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	{
		DrawLineAA(wood_anim[i].shift1.x,
			wood_anim[i].shift1.y,
			wood_anim[i].shift2.x,
			wood_anim[i].shift2.y, 0x00ff00, true);
	}

	//水
	SetDrawScreen(anim_handle[2]);
	ClearDrawScreen();
	DrawBox(0, 0, BOX_WIDTH, BOX_HEIGHT, 0x0000ff, true);
	for (int i = 0; i < ANIM_BLOCK_NUM; i++)
	{
		DrawBoxAA(water_anim[i].shift1.x,
				  water_anim[i].shift1.y,
				  water_anim[i].shift1.x + water_anim[i].erea.x,
				  water_anim[i].shift1.y + water_anim[i].erea.y, 0x0000ee, true);
	}

	//溶岩
	SetDrawScreen(stage_block_handle[0]);
	ClearDrawScreen();
	DrawBox(0, 0, BOX_WIDTH, BOX_HEIGHT, 0xff0000, true);
	for (int i = 0; i < 17; i++)
	{
		if (i % 2 == 0)
		{
			DrawCircleAA(5 , BOX_HEIGHT - (i * 4)+ (anim /3.7f), 8, 100, 0xff0000, TRUE);
			DrawCircleAA(15, BOX_HEIGHT - (i * 4)+ (anim /3.7f), 8, 100, 0xff0000, TRUE);
			DrawCircleAA(25, BOX_HEIGHT - (i * 4)+ (anim /3.7f), 8, 100, 0xff0000, TRUE);
			DrawCircleAA(35, BOX_HEIGHT - (i * 4)+ (anim /3.7f), 8, 100, 0xff0000, TRUE);
		}										 		  	
		else												
		{													
			DrawCircleAA(5 , BOX_HEIGHT - (i * 4)+ (anim /3.7f), 8, 100, 0xaa0000, TRUE);
			DrawCircleAA(15, BOX_HEIGHT - (i * 4)+ (anim /3.7f), 8, 100, 0xaa0000, TRUE);
			DrawCircleAA(25, BOX_HEIGHT - (i * 4)+ (anim /3.7f), 8, 100, 0xaa0000, TRUE);
			DrawCircleAA(35, BOX_HEIGHT - (i * 4)+ (anim /3.7f), 8, 100, 0xaa0000, TRUE);
		}
	}

	//水源
	SetDrawScreen(stage_block_handle[1]);
	ClearDrawScreen();
	DrawBox(0, 0, BOX_WIDTH, BOX_HEIGHT, 0xff0000, true);
	for (int i = 0; i < 12; i++)
	{
		if (i % 2 == 0)																			
		{																						
			DrawCircle(5  - (anim / 3), BOX_HEIGHT - (i * 4), 8, 0x0000ff, TRUE);		
			DrawCircle(15 - (anim / 3), BOX_HEIGHT - (i * 4), 8, 0x0000ff, TRUE);		
			DrawCircle(25 - (anim / 3), BOX_HEIGHT - (i * 4), 8, 0x0000ff, TRUE);		
			DrawCircle(35 - (anim / 3), BOX_HEIGHT - (i * 4), 8, 0x0000ff, TRUE);		
			DrawCircle(45 - (anim / 3), BOX_HEIGHT - (i * 4), 8, 0x0000ff, TRUE);		
			DrawCircle(55 - (anim / 3), BOX_HEIGHT - (i * 4), 8, 0x0000ff, TRUE);		
		}			  										
		else		  										
		{			 										
			DrawCircle(5  - (anim / 3), BOX_HEIGHT - (i * 4), 8, 0x0000aa, TRUE);
			DrawCircle(15 - (anim / 3), BOX_HEIGHT - (i * 4), 8, 0x0000aa, TRUE);
			DrawCircle(25 - (anim / 3), BOX_HEIGHT - (i * 4), 8, 0x0000aa, TRUE);
			DrawCircle(35 - (anim / 3), BOX_HEIGHT - (i * 4), 8, 0x0000aa, TRUE);
			DrawCircle(45 - (anim / 3), BOX_HEIGHT - (i * 4), 8, 0x0000aa, TRUE);
			DrawCircle(55 - (anim / 3), BOX_HEIGHT - (i * 4), 8, 0x0000aa, TRUE);
		}
	}

	//通常地面
	SetDrawScreen(stage_block_handle[2]);
	ClearDrawScreen();
	DrawBox(0, 0, BOX_WIDTH, BOX_HEIGHT, 0xff0000, true);
	for (int i = 0; i < 13; i++)
	{
		if (i % 2 == 0)
		{
			DrawCircle(5 , BOX_HEIGHT - (i * 4), 8, 0xaaaaaa, TRUE);
			DrawCircle(15, BOX_HEIGHT - (i * 4), 8, 0xaaaaaa, TRUE);
			DrawCircle(25, BOX_HEIGHT - (i * 4), 8, 0xaaaaaa, TRUE);
			DrawCircle(35, BOX_HEIGHT - (i * 4), 8, 0xaaaaaa, TRUE);
			DrawCircle(45, BOX_HEIGHT - (i * 4), 8, 0xaaaaaa, TRUE);
			DrawCircle(55, BOX_HEIGHT - (i * 4), 8, 0xaaaaaa, TRUE);
		}
		else
		{
			DrawCircle(5 , BOX_HEIGHT - (i * 4), 8, 0x888888, TRUE);
			DrawCircle(15, BOX_HEIGHT - (i * 4), 8, 0x888888, TRUE);
			DrawCircle(25, BOX_HEIGHT - (i * 4), 8, 0x888888, TRUE);
			DrawCircle(35, BOX_HEIGHT - (i * 4), 8, 0x888888, TRUE);
			DrawCircle(45, BOX_HEIGHT - (i * 4), 8, 0x888888, TRUE);
			DrawCircle(55, BOX_HEIGHT - (i * 4), 8, 0x888888, TRUE);
		}
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

void ResourceManager::StageAnimDraw(Vector2D _location, int _type)
{
	DrawGraphF(_location.x, _location.y, anim_handle[_type - 3], TRUE);	
}

void ResourceManager::StageBlockDraw(Vector2D _location, int _type)
{
	DrawGraphF(_location.x, _location.y, stage_block_handle[_type], TRUE);

}

int ResourceManager::SetGraph(const char* p)
{
	for (int i = 0; i < IMAGE_NUM; i++)
	{
		//空の配列に画像を格納する
		if (image_data[i] == NULL)
		{
			image_data[i] = LoadGraph(p);
			image_filepath[i] = const_cast<char*>(p);
			return i;
		}
		//同じ画像が既にあるならその格納場所を返す
		else if (image_filepath[i] == p)
		{
			return i;
		}
	}
	return -1;	//画像をどこにも格納できなかった場合
}

int ResourceManager::SetDivGraph(const char* p, int AllNum, int XNum, int YNum, int  XSize, int YSize,int AnimSpeed)
{
	for (int i = 0; i < DIV_IMAGE_NUM; i++)
	{
		//空の配列に分割画像を格納する
		if (div_image_data[i].div_image_handle[0] == NULL)
		{
			LoadDivGraph(p, AllNum, XNum, YNum, XSize, YSize, div_image_data[i].div_image_handle);
			div_image_data[i].div_image_filepath = const_cast<char*>(p);
			div_image_data[i].div_image_num = AllNum;
			div_image_data[i].anim_speed = AnimSpeed;
			return i;
		}
		//同じ画像が既にあるならその格納場所を返す
		else if (div_image_data[i].div_image_filepath == p)
		{
			return i;
		}
	}
	return -1;	//画像をどこにも格納できなかった場合
}

int ResourceManager::SetSound(const char* p)
{
	for (int i = 0; i < SOUND_NUM; i++)
	{
		//空の配列に音源を格納する
		if (sound_data[i] == NULL)
		{
			sound_data[i] = LoadSoundMem(p);
			ChangeVolumeSoundMem(200, sound_data[i]);
			sound_filepath[i] = const_cast<char*>(p);
			return i;
		}
		//同じ音源が既にあるならその格納場所を返す
		else if (sound_filepath[i] == p)
		{
			return i;
		}
	}
	return -1;	//音源をどこにも格納できなかった場合
}

int ResourceManager::GetGraph(int _num)
{
	return image_data[_num];
}

int ResourceManager::GetDivGraph(int _num1, int _num2)
{
	if (_num2 < 0 || div_image_data[_num1].div_image_num <= _num2)return -1;
	return div_image_data[_num1].div_image_handle[_num2];
}

void ResourceManager::DrawAnimGraph(Vector2D location, int _handle)
{
	DrawGraphF(location.x, location.y, div_image_data[_handle].div_image_handle[div_image_data[_handle].now_image], TRUE);
}

void ResourceManager::StartSound(int _num, bool roop_flg)
{
	// 再生周波数を設定する
	//SetFrequencySoundMem(sound_freq, sound_data[_num]);
	
	//再生
	if (roop_flg)
	{
		PlaySoundMem(sound_data[_num], DX_PLAYTYPE_LOOP, FALSE);
	}
	else
	{
		PlaySoundMem(sound_data[_num], DX_PLAYTYPE_BACK);
	}
}

void ResourceManager::StopSound(int _num)
{
	StopSoundMem(sound_data[_num]);
}

void ResourceManager::SetSoundVolume(int _num, int _volume)
{
	if (_volume > 255)
	{
		ChangeVolumeSoundMem(255, sound_data[_num]);

	}
	else if (_volume < 0)
	{
		ChangeVolumeSoundMem(0, sound_data[_num]);
	}
	else
	{
		ChangeVolumeSoundMem(_volume, sound_data[_num]);
	}
}

void ResourceManager::SetSoundFreq(int _num)
{
	sound_freq = _num;
}

void ResourceManager::DrawRotaBox(float _x, float _y, float _w, float _h, float _cx, float _cy, float _rad, int _color, bool _fill)
{
	float loc[4][2];
	float newloc[4][2];
	float rad;

	//４点のローカル座標を格納
	loc[0][0] = -(_w / 2) + _cx - _x;
	loc[0][1] =-(_h / 2)  + _cy - _y;
	loc[1][0] =(_w / 2)   + _cx - _x;
	loc[1][1] =-(_h / 2)  + _cy - _y;
	loc[2][0] =(_w / 2)   + _cx - _x;
	loc[2][1] =(_h / 2)   + _cy - _y;
	loc[3][0] = -(_w / 2) + _cx - _x;
	loc[3][1] =(_h / 2)   + _cy - _y;

	rad = (float)(_rad * M_PI / 180);

	for (int i = 0; i < 4; i++)
	{
		newloc[i][0] = (loc[i][0] * cosf(rad)) - (loc[i][1] * sinf(rad)) + _cx;
		newloc[i][1] = (loc[i][0] * sinf(rad)) + (loc[i][1] * cosf(rad)) + _cy;
	}

	DrawQuadrangleAA(newloc[0][0], newloc[0][1], newloc[1][0], newloc[1][1], newloc[2][0], newloc[2][1], newloc[3][0], newloc[3][1], _color, _fill);
}

void ResourceManager::DrawHeart(Vector2D l, Vector2D e)
{
	Vector2D circle[2];

	circle[0].x = l.x + ((e.x / 2) / 2);
	circle[0].y = l.y + ((e.y / 2) / 2);

	circle[1].x = l.x + (e.x / 2) + ((e.x / 2) / 2);
	circle[1].y = l.y + ((e.y / 2) / 2);

	Vector2D triangle[3];

	triangle[0].x = l.x;
	triangle[0].y = l.y + ((e.y / 2) / 2);

	triangle[1].x = l.x + e.x;
	triangle[1].y = l.y + ((e.y / 2) / 2);

	triangle[2].x = l.x + (e.x / 2);
	triangle[2].y = l.y + e.y ;


	for (int i = 0; i < 2; i++)
	{
		DrawCircleAA(circle[i].x, circle[i].y, ((e.x / 2) / 2) + 2, 64, 0xffffff, FALSE, 4.f);
	}
	
	for (int i = 0; i < 2; i++)
	{
		DrawCircleAA(circle[i].x, circle[i].y, ((e.x / 2) / 2), 64, 0xcc0000);
	}

	DrawTriangleAA(triangle[0].x - 1, triangle[0].y, triangle[1].x + 1, triangle[1].y, triangle[2].x, triangle[2].y, 0xcc0000, TRUE);
	DrawLineAA(triangle[0].x + 2, triangle[0].y + 9, triangle[2].x + 2, triangle[2].y + 1, 0xffffff, 4.f);
	DrawLineAA(triangle[1].x - 2, triangle[1].y + 9, triangle[2].x - 2, triangle[2].y + 1, 0xffffff, 4.f);
}
