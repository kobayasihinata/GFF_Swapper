#include "BossAttackWater.h"
#include "../Scene/GameMain.h"

#define _USE_MATH_DEFINES
#include <math.h>

BossAttackWater::BossAttackWater()
{
	type = WATER;
	can_swap = FALSE;
	can_hit = FALSE;
	is_boss_attack = TRUE;

	flg = false;
	count = 0;
	moveFlg = false;
	hitFlg = false;
	rad = 0.0f;
}

BossAttackWater::~BossAttackWater()
{
}

void BossAttackWater::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
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

void BossAttackWater::Update(GameMain* _g)
{
	__super::Update(_g);

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
				_g->CameraImpact(15);
				_g->SpawnEffect(location, erea, ExplosionEffect, 10, BLUE);
				ResourceManager::StartSound(end_se);
				_g->DeleteObject(object_pos, this);
			}
		}
	}
	else {
		//Location player = _g->GetPlayerLocation();
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
		_g->CameraImpact(15);
		_g->SpawnEffect(location, erea, ExplosionEffect, 10, BLUE);
		ResourceManager::StartSound(end_se);
		//ここで削除
		_g->DeleteObject(object_pos,this);
	}

	
}

void BossAttackWater::Draw() const
{
	if (flg) {
		for (int i = 0; i < 5; i++)
		{
			DrawCircleAA(local_location.x + (i * 6), local_location.y - (i * 6), erea.width - (i * 8), 32, GetColor(i * 25, i * 25, 255), TRUE);
		}
		//DrawCircleAA(local_location.x + 9, local_location.y - 12, erea.width - 10, 32, GetColor(255, 255, 255), TRUE);
	}
	DrawCircleAA(f_location.x, f_location.y, f_erea.width, 32, GetColor(100, 100, 255), TRUE);
	DrawCircleAA(f_location.x, f_location.y, f_erea.width-2, 32, GetColor(120, 120, 255), TRUE);

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
