#include "Cannon.h"
#include "../../Base/ObjectManager.h"

Cannon::Cannon(int _type, int _angle)
{
	cannon_type = _type;
	cannon_angle = _angle;

	type = CANNON;
	can_swap = FALSE;
	can_hit = TRUE;
	is_boss_attack = FALSE;
}

Cannon::~Cannon()
{

}

void Cannon::Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)
{
	location = _location;
	erea = _erea;
	color = _color_data;
	object_pos = _object_pos;
}

void Cannon::Finalize()
{

}

void Cannon::Update(ObjectManager* _manager)
{
	__super::Update(_manager);

	if (frame % 60 == 0)
	{
		switch (cannon_type)
		{
		case 0:
			_manager->CreateObject(new BossAttackFire(this->GetCenterLocation()), this->GetCenterLocation(), { 20,20 }, RED);
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
	DrawBox(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, 0xaaaaaa, TRUE);
}

void Cannon::Hit(Object* _object)
{

}

bool Cannon::SearchColor(Object* ob)
{
	return false;
}
