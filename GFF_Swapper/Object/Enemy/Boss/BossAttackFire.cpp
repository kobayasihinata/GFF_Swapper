#include "BossAttackFire.h"
#include "../../../Scene/GameMain.h"
#include "../../../Utility/ResourceManager.h"

BossAttackFire::BossAttackFire()
{
	camera = Camera::Get();
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

void BossAttackFire::Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)
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

void BossAttackFire::Update(ObjectManager* _manager)
{
	__super::Update(_manager);

	if (!flg) {
		//プレイヤーとボスの座標からベクトルを計算
		Vector2D player = _manager->GetPlayerLocation();
		Vector2D boss = { _manager->GetBossLocation()};
		Vector2D v;
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
		_manager->SpawnEffect(location, erea, ExplosionEffect, 10, RED);
		camera->SetImpact(15);
		ResourceManager::StartSound(end_se);
		if (this != nullptr) {
			_manager->DeleteObject(this);
		}
	}
}

void BossAttackFire::Draw() const
{
	ResourceManager::StageAnimDraw({ local_location.x - 20,local_location.y - 20 }, FIRE);
	DrawCircleAA(local_location.x, local_location.y, erea.x,100, 0xff0000, TRUE);
	ResourceManager::DrawRotaBox(local_location.x, local_location.y, erea.x, erea.y, local_location.x, local_location.y, 45, 0xff0000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120 + (frame%75));
	DrawCircleAA(local_location.x, local_location.y, erea.x,100, 0x000000, TRUE);
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
