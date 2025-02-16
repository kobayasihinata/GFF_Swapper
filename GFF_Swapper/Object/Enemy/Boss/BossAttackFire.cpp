#include "BossAttackFire.h"
#include "../../../Scene/GameMain.h"
#include "../../../Utility/ResourceManager.h"

BossAttackFire::BossAttackFire(Vector2D _parent_center_location, int _angle)
{
	this->parent_center_location = _parent_center_location;
	angle = _angle;
	camera = Camera::Get();
	object_type = FIRE;
	can_swap = FALSE;
	can_hit = FALSE;
	is_boss_attack = TRUE;

	set_velocity_once = false;
	hit_flg = false;
	player_hit = false;
	bound_cnt = 3;		//3回跳ね返ったら消えるようにする

	end_se = 0;
	start_se = 0;
}

BossAttackFire::~BossAttackFire()
{

}

void BossAttackFire::Initialize(Vector2D _location, Vector2D _erea, int _color_data)
{
	location = _location;
	color = _color_data;
	erea = _erea;

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

	//一回だけ移動方向計算
	if (!set_velocity_once) {

		SetAngle(_manager);

		set_velocity_once = true;
	}

	//画面端反射(X軸)
	if (local_location.x < 0 || local_location.x > 1280) {
		velocity.x = velocity.x * -1.f;
		bound_cnt--;
	}

	//画面端反射(Y軸)
	if (local_location.y < 0 || local_location.y > 720) {
		velocity.y = velocity.y * -1.f;
		if (local_location.y < 0) {
			local_location.y = 10.f;
		}
		if (local_location.y > 720) {
			local_location.y = 710.f;
		}
		bound_cnt--;
	}

	//移動量加算
	location.x += velocity.x;
	location.y += velocity.y;

	//反射回数上限を越したら破壊
	if (bound_cnt < 0) {
		hit_flg = true;
	}
	
	//破壊処理
	if (hit_flg) {
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
	//火球描画
	DrawCircleAA(local_location.x, local_location.y, erea.x,100, color, TRUE);
}

void BossAttackFire::Hit(Object* _object)
{
	//他のボス攻撃には干渉しない
	if (!_object->is_boss_attack)
	{
		//草ダメージゾーンに当たったら色だけ変える
		if (_object->object_type == WOOD &&
			!_object->can_swap)
		{
			_object->SetColorData(color);
			return;
		}
		//地面ブロックか木に当たったら色を上書きしてフラグを立てる
		if ((_object->object_type == GROUND_BLOCK || _object->object_type == WOOD) && _object->GetColorData() != WHITE && !_object->is_boss_attack) {
			_object->can_swap = TRUE;
			_object->SetColorData(color);
			hit_flg = true;
		}
	}
}

bool BossAttackFire::SearchColor(Object* ob)
{
	return false;
}

void BossAttackFire::SetAngle(ObjectManager* _manager)
{
	switch (angle)
	{
		//東
	case EAST:
		velocity = { 5,0 };
		break;
		//西
	case WEST:
		velocity = { -5,0 };
		break;
		//南
	case SOUTH:
		velocity = { 0,5 };
		break;
		//北
	case NORTH:
		velocity = { 0,-5 };
		break;
		//それ以外
	default:
		//プレイヤーの座標からベクトルを計算
		Vector2D player = { _manager->GetPlayerLocation().x + (_manager->GetPlayerErea().x / 2),
							_manager->GetPlayerLocation().y + (_manager->GetPlayerErea().y / 2) };
		float rad = atan2f(player.y - parent_center_location.y, player.x - parent_center_location.x);
		velocity.x = 5 * cosf(rad);
		velocity.y = 5 * sinf(rad);
		break;
	}

}
