#include "EffectSpawner.h"
#include "../Scene/GameMain.h"
#include "Stage.h"
#include "../Utility/common.h"
#include"../Utility/ResourceManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

EffectSpawner::EffectSpawner() :frame(0), swap_anim_timer(0),swap_se(0), change_effect_flg(false)
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		effect[i].spawn_flg = false;
		effect[i].timer = 0;						 
		effect[i].location = { 0,0 };				 
		effect[i].local_location = { 0.0 };			 
		effect[i].erea = { 0,0 };					 
		effect[i].effect_type = 0;					 
		effect[i].effect_time = 0;					 
		effect[i].touch_ground = false;				 
		effect[i].angle = 0.0f;						 
		effect[i].speed = 0.0f;						 
		effect[i].color = WHITE;					 
		effect[i].effect_shift[0] = { 0,0 };		 
		effect[i].effect_shift[1] = { 0,0 };		 
		effect[i].shine_timer = 0;					 
		effect[i].rad = 0.0f;						 
	}
}

EffectSpawner::~EffectSpawner()
{

}

void EffectSpawner::Initialize()
{
	camera = Camera::Get();

	for (int i = 0; i < EFFECT_NUM; i++)
	{
		ResetEffect(i);
	}
	swap_anim[0].move_flg = false;
	swap_anim[1].move_flg = false;	
	swap_anim_timer = 0;

	swap_se = ResourceManager::SetSound("Resource/Sounds/Effect/swap.wav");
	swap_move_se = ResourceManager::SetSound("Resource/Sounds/Effect/swap_move.wav");
}

void EffectSpawner::Update(ObjectManager* _manager)
{
	frame++;
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		if (effect[i].spawn_flg == true)
		{
			//ローカル座標の更新
			SetScreenPosition(camera->GetCameraLocation(), i);
			effect[i].timer++;
			switch (effect[i].effect_type)
			{
			//直進移動
			case 0:
			case 1:
			case 2:
				effect[i].location.x += effect[i].speed * cosf(effect[i].angle * (float)M_PI * 2);
				effect[i].location.y += effect[i].speed * sinf(effect[i].angle * (float)M_PI * 2);
				//指定された時間で消える
				if (effect[i].timer > effect[i].effect_time)
				{
					effect[i].spawn_flg = false;
				}
				break;
			//輝きエフェクト(移動あり)
			case 4:
				effect[i].location.x += effect[i].speed * cosf(effect[i].angle * (float)M_PI * 2);
				effect[i].location.y += effect[i].speed * sinf(effect[i].angle * (float)M_PI * 2);
			//輝きエフェクト
			case 3:
				if (effect[i].timer < effect[i].effect_time / 2)
				{
					effect[i].shine_timer += 2;
				}
				else
				{
					effect[i].shine_timer -= 2;
				}

				effect[i].rad = (float)(effect[i].timer * M_PI / 180);

				effect[i].effect_shift[0].x = (effect[i].shine_timer * cosf(effect[i].rad)) - (effect[i].shine_timer * sinf(effect[i].rad));
				effect[i].effect_shift[0].y = (effect[i].shine_timer * sinf(effect[i].rad)) + (effect[i].shine_timer * cosf(effect[i].rad));

				effect[i].rad = (float)((effect[i].timer+90) * M_PI / 180);

				effect[i].effect_shift[1].x = (effect[i].shine_timer * cosf(effect[i].rad)) - (effect[i].shine_timer * sinf(effect[i].rad));
				effect[i].effect_shift[1].y = (effect[i].shine_timer * sinf(effect[i].rad)) + (effect[i].shine_timer * cosf(effect[i].rad));

				//指定された時間で消える
				if (effect[i].timer > effect[i].effect_time)
				{
					effect[i].spawn_flg = false;
				}
				break;
			//オブジェクトが着地した時
			case 5:
				effect[i].location.x += effect[i].speed * cosf(effect[i].angle * (float)M_PI * 2);
				effect[i].location.y += effect[i].speed * sinf(effect[i].angle * (float)M_PI * 2);
				//指定された時間で消える
				if (effect[i].timer > effect[i].effect_time)
				{
					effect[i].spawn_flg = false;
				}
				break;
			//爆発
			case 6:
				//指定された時間で消える
				if (effect[i].timer > effect[i].effect_time)
				{
					effect[i].spawn_flg = false;
				}
				break;
			//ダメージ
			case 7:
				switch (effect[i].color)
				{
				case RED:
					effect[i].location.y += effect[i].speed;
					break;
				case GREEN:
					effect[i].location.y -= effect[i].speed;
					break;
				case BLUE:
					break;
				default:
					break;
				}
				if (effect[i].timer > effect[i].effect_time)
				{
					effect[i].spawn_flg = false;
				}
				break;

				//光が集まる
			case 8:
				effect[i].location.x += effect[i].speed * cosf(effect[i].angle);
				effect[i].location.y += effect[i].speed * sinf(effect[i].angle);

				//ゴールに近づいたら削除
				if (effect[i].location.x > effect[i].goal_location.x - 1 &&
					effect[i].location.x < effect[i].goal_location.x + 1 &&
					effect[i].location.y > effect[i].goal_location.y - 1 &&
					effect[i].location.y < effect[i].goal_location.y + 1)
				{
					effect[i].spawn_flg = false;
				}
				//一定時間経過しても削除
				//if (effect[i].timer > effect[i].effect_time)
				//{
				//	effect[i].spawn_flg = false;
				//}
				break;
			default:
				break;
			}
		}
	}

	//色交換エフェクトの更新
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < SWAP_EFFECT_NUM; j++)
		{
			//ローカル座標の更新
			swap_anim[i].local_location[j].x = swap_anim[i].location[j].x - camera->GetCameraLocation().x - (SWAP_EFFECT_SIZE / 2);
			swap_anim[i].local_location[j].y = swap_anim[i].location[j].y - camera->GetCameraLocation().y - (SWAP_EFFECT_SIZE / 2);
			//残像の更新
			//if (swap_anim[i].timer % 3 == 0)
			//{
				for (int k = 0; k < AFTERIMAGE_NUM; k++)
				{
					if (k + 1 < AFTERIMAGE_NUM)
					{
						swap_anim[i].old_location[j][k] = swap_anim[i].old_location[j][k + 1];
					}
					else
					{
						swap_anim[i].old_location[j][k] = swap_anim[i].local_location[j];
					}
				}
			//}
			//エフェクトの移動
			if (swap_anim[i].move_flg == true)
			{
				if (swap_anim[i].timer > ((float)SWAP_EFFECT_TIMER - 5))
				{
					swap_anim[i].location[j].x += (swap_anim[i].timer/5) * cosf(swap_anim[i].move_rad[j]);
					swap_anim[i].location[j].y += (swap_anim[i].timer/5) * sinf(swap_anim[i].move_rad[j]);
				}
				else if(swap_anim[i].timer < ((float)SWAP_EFFECT_TIMER - ((float)SWAP_EFFECT_TIMER / 2.9f)))
				{
					swap_anim[i].location[j].x += swap_anim[i].speed[j] * cosf(swap_anim[i].move_rad[j]);
					swap_anim[i].location[j].y += swap_anim[i].speed[j] * sinf(swap_anim[i].move_rad[j]);
				}
				SpawnEffect(swap_anim[i].location[j], swap_anim[i].erea, 1, 5, swap_anim[i].color);
				//radの更新
				if (swap_anim[i].timer < ((float)SWAP_EFFECT_TIMER - 5) && swap_anim[i].update_once[j] == false)
				{
					swap_anim[i].move_rad[j] = atan2f(swap_anim[i].goal.y - swap_anim[i].location[j].y,swap_anim[i].goal.x - swap_anim[i].location[j].x);
					swap_anim[i].speed[j] = (sqrtf(powf(fabsf(swap_anim[i].location[j].x - swap_anim[i].goal.x), 2) + powf(fabsf(swap_anim[i].location[j].y - swap_anim[i].goal.y), 2)) / (((float)SWAP_EFFECT_TIMER - ((float)SWAP_EFFECT_TIMER / 2.9f)) / 1.5f));
					swap_anim[i].update_once[j] = true;
					ResourceManager::StartSound(swap_move_se);
				}
			}
		}
		if (swap_anim[i].move_flg == true && --swap_anim[i].timer < SWAP_EFFECT_STOP_TIME)
		{
			swap_anim[i].move_flg = false;
			//色交換完了エフェクト
			//_manager->SpawnEffect(swap_anim[i].goal, swap_anim[i].erea, PlayerSpawnEffect, 30, swap_anim[i].color);

		}
	}
	if (swap_anim[0].timer < SWAP_EFFECT_STOP_TIME)
	{
		swap_anim_timer++;
	}
	else
	{
		swap_anim_timer = 0;
	}
}

void EffectSpawner::Draw()const
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		if (effect[i].spawn_flg == true)
		{
			//各エフェクトの描画
			switch (effect[i].effect_type)
			{
				//四角形直進移動
			case 0:
				DrawBoxAA(effect[i].local_location.x, effect[i].local_location.y, effect[i].local_location.x + effect[i].erea.x, effect[i].local_location.y + effect[i].erea.y, effect[i].color, TRUE);
				break;
				//回転四角形直進移動
			case 1:
				ResourceManager::DrawRotaBox(effect[i].local_location.x, effect[i].local_location.y, effect[i].erea.x, effect[i].erea.y, effect[i].local_location.x, effect[i].local_location.y, (float)effect[i].timer * 10, effect[i].color, TRUE);
				break;
				//四角形直進移動(フェードアウト)
			case 2:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (effect[i].timer * (255/effect[i].effect_time)));
				DrawBoxAA(effect[i].local_location.x, effect[i].local_location.y, effect[i].local_location.x + effect[i].erea.x, effect[i].local_location.y + effect[i].erea.y, effect[i].color, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				break;
				//輝き
			case 3:
			case 4:
				DrawLineAA(effect[i].local_location.x - (effect[i].effect_shift[0].x / 2),
					effect[i].local_location.y - (effect[i].effect_shift[0].y / 2),
					effect[i].local_location.x + (effect[i].effect_shift[0].x / 2),
					effect[i].local_location.y + (effect[i].effect_shift[0].y / 2), effect[i].color, TRUE);
				DrawLineAA(effect[i].local_location.x - (effect[i].effect_shift[1].x / 2),
					effect[i].local_location.y - (effect[i].effect_shift[1].y / 2),
					effect[i].local_location.x + (effect[i].effect_shift[1].x / 2),
					effect[i].local_location.y + (effect[i].effect_shift[1].y / 2), effect[i].color, TRUE);
				break;
				//着地した時
			case 5:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (effect[i].timer * (255 / effect[i].effect_time)));
				DrawBoxAA(effect[i].local_location.x, effect[i].local_location.y, effect[i].local_location.x + effect[i].erea.x, effect[i].local_location.y + effect[i].erea.y, effect[i].color, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				break;
				//爆発した時
			case 6:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (effect[i].timer * (255 / effect[i].effect_time)));
				DrawCircleAA(effect[i].local_location.x, effect[i].local_location.y, (float)effect[i].timer * 15, 100, effect[i].color);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				break;
			case 7:
				//ダメージを受けた時のプレイヤーの色に応じて挙動を変える
				switch (effect[i].color)
				{
				case RED:	//赤の時ダメージ（水の演出）
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (effect[i].timer * (255 / effect[i].effect_time)));
					DrawCircleAA(effect[i].local_location.x, effect[i].local_location.y, (float)effect[i].effect_time - (float)effect[i].timer, 100, BLUE);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					break;
				case GREEN:	//緑の時ダメージ（火の演出）
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (effect[i].timer * (255 / effect[i].effect_time)));
					DrawCircleAA(effect[i].local_location.x, effect[i].local_location.y, (float)effect[i].effect_time - (float)effect[i].timer, 100, RED);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					break;
				case BLUE:	//青の時ダメージ（木の演出）
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (effect[i].timer * (255 / effect[i].effect_time)));
					DrawCircleAA(effect[i].local_location.x, effect[i].local_location.y, (float)effect[i].effect_time - (float)effect[i].timer,100, GREEN);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					break;
				default:
					break;
				}
				break;

				//光が集まる
			case 8:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, (sqrtf(powf(effect[i].location.x - effect[i].goal_location.x, 2) + powf(effect[i].location.y - effect[i].goal_location.y,2)))*4);
				ResourceManager::DrawRotaBox(effect[i].local_location.x, effect[i].local_location.y, effect[i].erea.x, effect[i].erea.y, effect[i].local_location.x, effect[i].local_location.y, (float)effect[i].timer * 10, effect[i].color, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				break;
			default:
				break;
			}
		}
	}

	//色交換エフェクトの描画
	for (int i = 0; i < 2; i++)
	{
		if (swap_anim[i].move_flg == true)
		{
			for (int j = 0; j < SWAP_EFFECT_NUM; j++)
			{
				//回転四角形描画
				ResourceManager::DrawRotaBox(swap_anim[i].local_location[j].x, 
					swap_anim[i].local_location[j].y, 
					SWAP_EFFECT_SIZE, 
					SWAP_EFFECT_SIZE, 
					swap_anim[i].local_location[j].x, 
					swap_anim[i].local_location[j].y, 
					(float)effect[i].timer * 10, 
					swap_anim[i].color, TRUE);

				for (int k = 0; k < AFTERIMAGE_NUM; k++)
				{
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 -(10 - (k*2))*10);
					//ResourceManager::DrawRotaBox(swap_anim[i].old_location[j][k].x, 
					//	swap_anim[i].old_location[j][k].y, 
					//	SWAP_EFFECT_SIZE, 
					//	SWAP_EFFECT_SIZE, 
					//	swap_anim[i].old_location[j][k].x, 
					//	swap_anim[i].old_location[j][k].y, 
					//	(float)(effect[i].timer - (5 -k)) * 10, 
					//	swap_anim[i].color, TRUE);
					if (swap_anim[i].old_location[j][k].x != 0)
					{
						if (k + 1 < AFTERIMAGE_NUM)
						{
							DrawLineAA(swap_anim[i].old_location[j][k].x, swap_anim[i].old_location[j][k].y, swap_anim[i].old_location[j][k + 1].x, swap_anim[i].old_location[j][k + 1].y, swap_anim[i].color, TRUE);
						}
						else
						{
							DrawLineAA(swap_anim[i].old_location[j][k].x, swap_anim[i].old_location[j][k].y, swap_anim[i].local_location[j].x, swap_anim[i].local_location[j].y, swap_anim[i].color, TRUE);
						}
					}
				}
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}
		}
		if (swap_anim_timer > 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 - (swap_anim_timer * 20));
			DrawBoxAA(swap_anim[i].goal_local.x - (swap_anim_timer * 10), swap_anim[i].goal_local.y - (swap_anim_timer * 10), swap_anim[i].goal_local.x + swap_anim[i].erea.x + (swap_anim_timer * 5), swap_anim[i].goal_local.y + swap_anim[i].erea.y + (swap_anim_timer * 5), swap_anim[i].color, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}
}

void EffectSpawner::Finalize()
{

}

void EffectSpawner::SpawnEffect(Vector2D _location, Vector2D _erea,int _effect_type, int _time, int _color, float _angle)
{
	switch (_effect_type)
	{
	//プレイヤースポーンエフェクト
	case PlayerSpawnEffect:
		for (float j = 0.0f; j < 1; j += 0.125f)
		{
			SpawnParticle(
				_location,
				_erea,
				{0,0},
				2,
				_time,
				2,
				_color,
				j
			);
		}
		break;
	//輝き
	case ShineEffect:
		SpawnParticle(
			{ _location.x + GetRand((int)(_erea.x)),_location.y + GetRand((int)(_erea.y))},
			_erea,
			{0,0},
			3,
			_time,
			1,
			_color,
			0
		);
		break;
		//塵になって消える
	case DeathEffect:
		SpawnParticle(
			{ _location.x + GetRand((int)(_erea.x)),_location.y + GetRand((int)(_erea.y))},
			_erea,
			{0,0},
			4,
			_time,
			1,
			_color,
			(float)(GetRand(100) / 100.0f)
		);
		break;
		//着地エフェクト
	case LandingEffect:
		for (int i = 0; i < 7; i++)
		{
			SpawnParticle(
				{ _location.x + (_erea.x / 2) + GetRand(10),_location.y + _erea.y },
				{ 10,5 },
				{0,0},
				5,
				_time,
				5,
				_color,
				(float)((48+GetRand(12) + (GetRand(1) * 38)) / 100.0f)
			);
		}
		break;
		//爆発エフェクト
	case ExplosionEffect:
		SpawnParticle(
			_location,
			_erea,
			{0,0},
			6,
			_time,
			1,
			_color,
			0
		);
		break;
		//ダメージエフェクト
	case DamageEffect:
		for (int i = 0; i < 10; i++)
		{
			SpawnParticle(
				{ _location.x + GetRand((int)_erea.x),_location.y + GetRand((int)_erea.y) },
				{ 7,7 },
				{0,0},
				7,
				_time,
				1,
				_color,
				_angle
			);
		}
		break;

		//光が集まるエフェクト
	case LightGathers:
		SpawnParticle(
			{ _location.x -(_erea.x/2) + GetRand((int)_erea.x),_location.y-(_erea.y/2) + GetRand((int)_erea.y)},
			{ 5,5 },
			_location,
			8,
			_time,
			1,
			_color,
			_angle
		);
		break;
	default:
		break;
}
}

void EffectSpawner::SetScreenPosition(Vector2D _world_to_screen,int _num)
{
	effect[_num].local_location.x = effect[_num].location.x - _world_to_screen.x;
	effect[_num].local_location.y = effect[_num].location.y - _world_to_screen.y;
}

void EffectSpawner::ResetEffect(int _num)
{
	effect[_num].spawn_flg = false;
	effect[_num].timer = 0;
	effect[_num].location = { 0,0 };
	effect[_num].local_location = { 0.0 };
	effect[_num].erea = { 0,0 };
	effect[_num].effect_type = 0;
	effect[_num].effect_time = 0;
	effect[_num].touch_ground = false;
	effect[_num].angle = 0.0f;
	effect[_num].speed = 0.0f;
	effect[_num].color = WHITE;
	effect[_num].effect_shift[0] = { 0,0 };
	effect[_num].effect_shift[1] = { 0,0 };
	effect[_num].shine_timer = 0;
	effect[_num].rad = 0.0f;
}

int EffectSpawner::Swap(Object* _object1, Object* _object2)
{
	for (int i = 0; i < SWAP_EFFECT_NUM; i++)
	{
		swap_anim[0].location[i] = _object1->GetCenterLocation();
		swap_anim[1].location[i] = _object2->GetCenterLocation();
		swap_anim[0].move_rad[i] = (float)GetRand(628)/100;
		swap_anim[1].move_rad[i] = (float)GetRand(628)/100;
		swap_anim[0].update_once[i] = false;
		swap_anim[1].update_once[i] = false;
		for (int j = 0; j < AFTERIMAGE_NUM; j++)
		{
			swap_anim[0].old_location[i][j] = { 0.0 };
			swap_anim[1].old_location[i][j] = { 0.0 };
		}
	}
	swap_anim[0].default_rad = atan2f(_object2->GetCenterLocation().y - _object1->GetCenterLocation().y,
							   _object2->GetCenterLocation().x - _object1->GetCenterLocation().x);
	swap_anim[1].default_rad = atan2f(_object1->GetCenterLocation().y - _object2->GetCenterLocation().y,
							   _object1->GetCenterLocation().x - _object2->GetCenterLocation().x);
	swap_anim[0].start = _object1->GetCenterLocation();
	swap_anim[1].start = _object2->GetCenterLocation();
	swap_anim[0].goal = _object2->GetCenterLocation();
	swap_anim[1].goal = _object1->GetCenterLocation();
	swap_anim[0].goal_local = { _object2->GetLocalLocation().x + (_object2->GetErea().x / 2), _object2->GetLocalLocation().y + (_object2->GetErea().y / 2) };
	swap_anim[1].goal_local = { _object1->GetLocalLocation().x + (_object1->GetErea().x / 2), _object1->GetLocalLocation().y + (_object1->GetErea().y / 2) };
	swap_anim[0].erea = { SWAP_EFFECT_SIZE ,SWAP_EFFECT_SIZE };
	swap_anim[1].erea = { SWAP_EFFECT_SIZE ,SWAP_EFFECT_SIZE };
	swap_anim[0].move_flg = true;
	swap_anim[1].move_flg = true;
	swap_anim[0].color = _object1->GetColorData();
	swap_anim[1].color = _object2->GetColorData();

	//移動にかかる時間測定
	Vector2D move;
	float g;
	move.x = fabsf(_object1->GetCenterLocation().x - _object2->GetCenterLocation().x);
	move.y = fabsf(_object1->GetCenterLocation().y - _object2->GetCenterLocation().y);
	g = powf(move.x, 2) + powf(move.y, 2);

	//swap_anim[0].timer = (int)((sqrtf(g) / swap_anim[0].speed) * 0.9f) + SWAP_EFFECT_STOP_TIME;
	swap_anim[0].timer = SWAP_EFFECT_TIMER + SWAP_EFFECT_STOP_TIME;
	if (swap_anim[0].timer < 0)
	{
		swap_anim[0].timer = 0;
	}
	swap_anim[1].timer = swap_anim[0].timer;

	ResourceManager::SetSoundFreq(DEFAULT_FREQ);
	ResourceManager::StartSound(swap_se);

	return swap_anim[0].timer;
}

void EffectSpawner::SpawnParticle(Vector2D _location, Vector2D _erea, Vector2D _goal_location, int _type, int _time, float _speed,int _color, float _angle)
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		if (effect[i].spawn_flg == false)
		{
			ResetEffect(i);
			effect[i].spawn_flg = true;
			effect[i].location = _location;
			effect[i].erea = _erea;
			effect[i].goal_location = _goal_location;
			effect[i].effect_type = _type;
			effect[i].effect_time = _time;
			effect[i].speed = _speed;
			effect[i].color = _color;
			effect[i].angle = _angle;
			//光が集まるエフェクトだけここで角度計算
			if (_type == 8)
			{
				effect[i].angle = atan2f(effect[i].goal_location.y - effect[i].location.y, effect[i].goal_location.x - effect[i].location.x);
			}
			break;
		}
	}
}


