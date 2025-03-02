#include "Cannon.h"
#include "../../Base/ObjectManager.h"
#include "../../../Utility/ResourceManager.h"

Cannon::Cannon(int _type, int _angle)
{
	camera = Camera::Get();
	cannon_type = _type;
	cannon_angle = _angle;
	cannon_stop = false;

	object_type = CANNON;
	can_swap = FALSE;
	can_hit = TRUE;
	is_boss_attack = FALSE;
}

Cannon::~Cannon()
{

}

void Cannon::Initialize(Vector2D _location, Vector2D _erea, int _color_data)
{
	location = _location;
	erea = _erea;
	color = _color_data;

	cannon_image = ResourceManager::SetDivGraph("Resource/Images/sozai/cannon.png", 12, 4, 3, 40, 40, 0);
}

void Cannon::Finalize()
{

}

void Cannon::Update(ObjectManager* _manager)
{
	__super::Update(_manager);

	//大砲停止フラグリセット
	cannon_stop = false;
	switch (cannon_type)
	{
		//火
	case 0:
		//弾の向き設定 
		if (this->location.x - CANNON_STOP > camera->GetPlayerLocation().x + PLAYER_WIDTH)
		{
			//大砲より左にプレイヤーがいるので、西に向けて弾を撃つ
			cannon_angle = WEST;
		}
		else if (this->location.x + this->erea.x + CANNON_STOP < camera->GetPlayerLocation().x)
		{
			//大砲より右にプレイヤーがいるので、東に向けて弾を撃つ
			cannon_angle = EAST;
		}
		else if (this->location.y - CANNON_STOP > camera->GetPlayerLocation().y + PLAYER_HEIGHT)
		{
			//大砲より上にプレイヤーがいるので、北に向けて弾を撃つ
			cannon_angle = NORTH;
		}
		else if (this->location.y + this->erea.y + CANNON_STOP < camera->GetPlayerLocation().y)
		{
			//大砲より下にプレイヤーがいるので、南に向けて弾を撃つ
			cannon_angle = SOUTH;
		}
		else
		{
			//プレイヤーが大砲に近いので、大砲を止める
			cannon_stop = true;
		}
		break;

		//木
	case 1:
		//木のキャノンは上固定
		cannon_angle = NORTH;
		break;
		//水
	case 2:
		//弾の向き設定 
		if (this->location.x - CANNON_STOP < camera->GetPlayerLocation().x + PLAYER_WIDTH &&
			this->location.x + this->erea.x + CANNON_STOP > camera->GetPlayerLocation().x &&
			this->location.y - CANNON_STOP < camera->GetPlayerLocation().y + PLAYER_HEIGHT &&
			this->location.y + this->erea.y + CANNON_STOP > camera->GetPlayerLocation().y)
		{
			//プレイヤーが大砲に近いので、大砲を止める
			cannon_stop = true;
		}
		if (this->location.x > camera->GetPlayerLocation().x)
		{
			//大砲より左にプレイヤーがいるので、西に向けて弾を撃つ
			cannon_angle = WEST;
		}
		else if (this->location.x < camera->GetPlayerLocation().x)
		{
			//大砲より右にプレイヤーがいるので、東に向けて弾を撃つ
			cannon_angle = EAST;
		}
		break;
	}

	//大砲を弾の種類別のクールタイム毎に発射する
	if (!cannon_stop && (frame % cannon_cooldown[cannon_type] == 0 || frame == 1))
	{
		switch (cannon_type)
		{
		case 0:
			_manager->CreateObject(new BossAttackFire(this->GetCenterLocation(),cannon_angle), this->GetCenterLocation(), { 20,20 }, RED);
			break;
		case 1:
			_manager->CreateObject(new BossAttackWood, { this->GetCenterLocation().x - 20,this->GetCenterLocation().y + 20 }, { 1000.0f,40.f }, GREEN);
			break;
		case 2:
			_manager->CreateObject(new BossAttackWater, this->GetCenterLocation(), { 40.f,40.f }, BLUE);
			break;
		default:
			break;
		}
	}
}

void Cannon::Draw()const
{
	DrawGraphF(local_location.x, local_location.y, ResourceManager::GetDivGraph(cannon_image, cannon_angle+(cannon_type*4)),true);
	if (!cannon_stop)
	{
		//発射周期の視覚化
		DrawCircleAA(local_location.x + (erea.x / 2),
			local_location.y + (erea.y / 2),
			(cannon_cooldown[cannon_type] - (frame % cannon_cooldown[cannon_type])) * 1.5f, 100, ColorList[cannon_type], false);
	}
#ifdef _DEBUG
	//大砲停止範囲の描画
	DrawBox(local_location.x - CANNON_STOP, 
			local_location.y - CANNON_STOP, 
			local_location.x + erea.x + CANNON_STOP, 
			local_location.y + erea.y + CANNON_STOP, 0xff0000, false);
#endif // _DEBUG

}

void Cannon::Hit(Object* _object)
{

}

bool Cannon::SearchColor(Object* ob)
{
	return false;
}
