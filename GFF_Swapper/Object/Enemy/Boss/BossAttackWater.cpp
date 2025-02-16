#include "BossAttackWater.h"
#include "../../../Scene/GameMain.h"

#define _USE_MATH_DEFINES
#include <math.h>

BossAttackWater::BossAttackWater()
{
	camera = Camera::Get();
	object_type = WATER;
	can_swap = FALSE;
	can_hit = FALSE;
	is_boss_attack = TRUE;

	set_velocity_once = false;
	move_direction = false;
	hit_flg = false;
	player_hit = false;
	rad = 0.0f;
}

BossAttackWater::~BossAttackWater()
{

}

void BossAttackWater::Initialize(Vector2D _location, Vector2D _erea, int _color_data)
{
	location = _location;
	color = _color_data;
	erea = _erea;

	f_location = _location;
	f_erea = { 10,10 };

	start_se = ResourceManager::SetSound("Resource/Sounds/Enemy/Boss/attack_start_water.wav");
	end_se = ResourceManager::SetSound("Resource/Sounds/Enemy/Boss/attack_end_water.wav");

	ResourceManager::StartSound(start_se);


}

void BossAttackWater::Finalize()
{

}

void BossAttackWater::Update(ObjectManager* _manager)
{
	__super::Update(_manager);

	if (set_velocity_once) {
		//一定角度に達したら切り返し
		if (move_direction) {
			if (velocity.y < -4.f) {
				move_direction = false;
			}
			velocity.y -= 0.1f;
		}
		else {
			if (velocity.y > 4.f) {
				move_direction = true;
			}
			velocity.y += 0.1f;
		}

		//移動量加算
		location.x += velocity.x;
		location.y += velocity.y;

		//端に到達したら破壊
		if (local_location.x < 0 || local_location.x > 1280 || local_location.y < 0 || local_location.y > 720) {
			if (this != nullptr) {
				camera->SetImpact(15);
				_manager->SpawnEffect(location, erea, ExplosionEffect, 10, BLUE);
				ResourceManager::StartSound(end_se);
				_manager->DeleteObject(this);
			}
		}
	}
	else {
		//移動方向決定
		if(local_location.x < (SCREEN_WIDTH/2)){
			velocity.x = 3.f;
		}
		else {
			velocity.x = -3.f;
		}
		velocity.y = (float)(GetRand(6) - 3);
		set_velocity_once = true;
	}
	//水球内の球回転計算
	rad += 0.02f;
	if (rad > 6.28f)
	{
		rad = 0.0f;
	}
	f_location = local_location;
	//radに応じた向きに進める
	f_location.x += 30 * cosf(5.0f * (frame / 5) + (float)M_PI) * cosf(rad);
	f_location.y += 30 * cosf(5.0f * (frame / 5) + (float)M_PI) * sinf(rad);
	if (hit_flg) {
		camera->SetImpact(15);
		_manager->SpawnEffect(location, erea, ExplosionEffect, 10, BLUE);
		ResourceManager::StartSound(end_se);
		//ここで削除
		_manager->DeleteObject(this);
	}
}

void BossAttackWater::Draw() const
{
	if (set_velocity_once) {
		if (!player_hit)
		{
			for (int i = 0; i < 5; i++)
			{
				DrawCircleAA(local_location.x + (i * 6), local_location.y - (i * 6), erea.x - (i * 8), 32, GetColor(i * 25, i * 25, 255), TRUE);
			}
			DrawCircleAA(f_location.x, f_location.y, f_erea.x, 32, GetColor(100, 100, 255), TRUE);
			DrawCircleAA(f_location.x, f_location.y, f_erea.x - 2, 32, GetColor(120, 120, 255), TRUE);
		}
		else
		{
			DrawCircleAA(local_location.x, local_location.y, 40, 32, 0x00ff00, TRUE);

		}
	}
}

void BossAttackWater::Hit(Object* _object)
{
	//他のボス攻撃には干渉しない
	if (!_object->is_boss_attack)
	{
		//火ダメージゾーンに当たったら色だけ変える
		if (_object->object_type == FIRE &&
			!_object->can_swap)
		{
			_object->SetColorData(color);
			return;
		}
		//地面ブロックもしくは火ブロックを水ブロックに変える
		if ((_object->object_type == GROUND_BLOCK || _object->object_type == FIRE) && _object->GetColorData() != WHITE && _object->is_boss_attack == FALSE) {
			_object->can_swap = TRUE;
			_object->SetColorData(color);
			hit_flg = true;
		}
	}
}

bool BossAttackWater::SearchColor(Object* ob)
{
	return false;
}
