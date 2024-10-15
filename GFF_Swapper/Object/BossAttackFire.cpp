#include "BossAttackFire.h"
#include "../Scene/GameMain.h"
#include "../Utility/ResourceManager.h"

BossAttackFire::BossAttackFire()
{
	type = FIRE;
	can_swap = FALSE;
	can_hit = FALSE;
	is_boss_attack = TRUE;

	flg = false;
	hitFlg = false;

	boundCnt = 3;
}

BossAttackFire::~BossAttackFire()
{

}

void BossAttackFire::Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)
{
	location = _location;
	color = _color_data;
	erea = _erea;
	object_pos = _object_pos;

	start_se = ResourceManager::SetSound("Resource/Sounds/Enemy/Boss/attack_start_fire.wav");
	end_se = ResourceManager::SetSound("Resource/Sounds/Enemy/Boss/attack_end_fire.wav");

	ResourceManager::StartSound(start_se);

}

void BossAttackFire::Finalize()
{
}

void BossAttackFire::Update(GameMain* _g)
{
	__super::Update(_g);

	if (!flg) {
		//プレイヤーとボスの座標からベクトルを計算
		Location player = _g->GetPlayerLocation();
		Location boss = { _g->GetBossLocation()};
		Location v;
		float len = sqrtf(powf(player.x - boss.x, 2) + powf(player.y - boss.y + (GetRand(6) - 3), 2));
		v.x = (player.x - boss.x) / len;
		v.y = (player.y - boss.y) / len;
		
		velocity.x = v.x * 5;
		velocity.y = v.y * 5;

		flg = true;
	}

	if (local_location.x < 0 || local_location.x > 1280) {
		velocity.x = velocity.x * -1.f;
		boundCnt--;
	}

	if (local_location.y < 0 || local_location.y > 720) {
		velocity.y = velocity.y * -1.f;
		if (local_location.y < 0) {
			local_location.y = 10.f;
		}
		if (local_location.y > 720) {
			local_location.y = 710.f;
		}
		boundCnt--;
	}

	location.x += velocity.x;
	location.y += velocity.y;

	if (boundCnt < 0) {
		hitFlg = true;
	}
	

	if (hitFlg) {
		_g->SpawnEffect(location, erea, ExplosionEffect, 10, RED);
		_g->CameraImpact(15);
		ResourceManager::StartSound(end_se);
		if (this != nullptr) {
			_g->DeleteObject(object_pos,this);
		}
	}
}

void BossAttackFire::Draw() const
{
	ResourceManager::StageAnimDraw({ local_location.x - 20,local_location.y - 20 }, FIRE);
	DrawCircleAA(local_location.x, local_location.y, erea.width,100, 0xff0000, TRUE);
	ResourceManager::DrawRotaBox(local_location.x, local_location.y, erea.width, erea.height, local_location.x, local_location.y, 45, 0xff0000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120 + (frame%75));
	DrawCircleAA(local_location.x, local_location.y, erea.width,100, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void BossAttackFire::Hit(Object* _object)
{
	if ((_object->GetObjectType() == BLOCK || _object->GetObjectType() == WOOD) && _object->GetColorData() != WHITE) {
		_object->SetCanSwap(TRUE);
		_object->SetColorData(color);
	}
}

bool BossAttackFire::SearchColor(Object* ob)
{
	return false;
}
