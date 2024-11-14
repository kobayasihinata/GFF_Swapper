#include "BossAttackWater.h"
#include "../../../Scene/GameMain.h"

#define _USE_MATH_DEFINES
#include <math.h>

BossAttackWater::BossAttackWater()
{
	camera = Camera::Get();
	type = WATER;
	can_swap = FALSE;
	can_hit = FALSE;
	is_boss_attack = TRUE;

	flg = false;
	count = 0;
	moveFlg = false;
	hitFlg = false;
	player_hit = false;
	rad = 0.0f;
}

BossAttackWater::~BossAttackWater()
{

}

void BossAttackWater::Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)
{
	location = _location;
	color = _color_data;
	erea = _erea;
	object_pos = _object_pos;

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

	if (flg) {
		if (moveFlg) {
			if (velocity.y < -4.f) {
				moveFlg = false;
			}
			velocity.y -= 0.1f;
		}
		else {
			if (velocity.y > 4.f) {
				moveFlg = true;
			}
			velocity.y += 0.1f;
		}

		location.x += velocity.x;
		location.y += velocity.y;

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
		//Location player = _manager->GetPlayerLocation();
		//if (player.x - location.x > 0) {
		if(location.x < 640.f){
			velocity.x = 3.f;
		}
		else {
			velocity.x = -3.f;
		}
		velocity.y = (float)(GetRand(6) - 3);
		flg = true;
	}
	rad += 0.02f;
	if (rad > 6.28f)
	{
		rad = 0.0f;
	}
	f_location = local_location;
	//radに応じた向きに進める
	f_location.x += 30 * cosf(5.0f * (frame / 5) + (float)M_PI) * cosf(rad);
	f_location.y += 30 * cosf(5.0f * (frame / 5) + (float)M_PI) * sinf(rad);
	if (hitFlg) {
		camera->SetImpact(15);
		_manager->SpawnEffect(location, erea, ExplosionEffect, 10, BLUE);
		ResourceManager::StartSound(end_se);
		//ここで削除
		_manager->DeleteObject(this);
	}
}

void BossAttackWater::Draw() const
{
	if (flg) {
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
	if ((_object->GetObjectType() == BLOCK || _object->GetObjectType() == FIRE) && _object->GetColorData() != WHITE && _object->GetIsBossAttack() == FALSE) {
		_object->SetCanSwap(TRUE);
		_object->SetColorData(color);
		hitFlg = true;
	}
}

bool BossAttackWater::SearchColor(Object* ob)
{
	return false;
}
