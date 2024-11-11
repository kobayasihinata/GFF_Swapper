#include "BossAttackWood.h"
#include "../../../Scene/GameMain.h"
#include "../../../Utility/ResourceManager.h"

BossAttackWood::BossAttackWood()
{
	camera = Camera::Get();
	count = 0;
	w_type = 0;
	camera_impact_once = false;
	
	type = WOOD;
	can_swap = FALSE;
	can_hit = FALSE;
	is_boss_attack = TRUE;
}

BossAttackWood::~BossAttackWood()
{
}

void BossAttackWood::Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)
{
	location.x = _location.x;
	location.y = _location.y;
	color = _color_data;

	object_pos = _object_pos;

	velocity.x = 0.f;
	velocity.y = -10.f;

	//erea.y = 0.f;
	erea.y = _erea.x;//_erea.y;
	erea.x = _erea.y;
	//bambooHeight = _location.y + _erea.y;
	bambooHeight = _erea.y;

	knot = (int)(_erea.y / 40.f);

	startLoc = _location;

	start_se = ResourceManager::SetSound("Resource/Sounds/Enemy/Boss/attack_wood.wav");

}

void BossAttackWood::Finalize()
{
}

void BossAttackWood::Update(ObjectManager* _manager)
{
	__super::Update(_manager);

	if (camera_impact_once == false)
	{
		camera->SetImpact(15);
		ResourceManager::StartSound(start_se);
		camera_impact_once = true;
	}
	

	MoveBamboo();
	if (velocity.y == 0.f) {
		if (count++ > 180) {
			if (this != nullptr) {
				_manager->DeleteObject(this);
			}
		}
	}
}

void BossAttackWood::Draw() const
{
	for (int i = 0; i < (erea.y / BOX_HEIGHT); i++)
	{
		//ResourceManager::StageAnimDraw({ local_location.x, local_location.y + (i * BOX_HEIGHT) }, WOOD);
		DrawBoxAA(local_location.x + 3, local_location.y + (i * BOX_HEIGHT), local_location.x + erea.x - 3, local_location.y + erea.y + (i * BOX_HEIGHT), 0x00cc00, true);
		DrawBoxAA(local_location.x + 2, local_location.y + (i * BOX_HEIGHT), local_location.x + erea.x - 2, local_location.y + 2 + (i * BOX_HEIGHT), 0x00ff00, true);
		DrawBoxAA(local_location.x + 2, local_location.y + erea.y + (i * BOX_HEIGHT), local_location.x + erea.x - 2, local_location.y + erea.y - 2 + (i * BOX_HEIGHT), 0x00ff00, true);
		DrawBoxAA(local_location.x + 10, local_location.y + 2 + (i * BOX_HEIGHT), local_location.x + 13, local_location.y + erea.y - 2 + (i * BOX_HEIGHT), 0x00ee00, true);
		if(player_hit)ResourceManager::StageAnimDraw({ local_location.x,local_location.y + (i * BOX_HEIGHT), } , FIRE);
		if(player_hit)ResourceManager::StageAnimDraw({ local_location.x,local_location.y + (i * BOX_HEIGHT), } , FIRE);

	}

	//DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, color, TRUE);
}

void BossAttackWood::Hit(Object* _object)
{
	if ((_object->GetObjectType() == BLOCK || _object->GetObjectType() == WATER) && _object->GetColorData() != WHITE) {
		_object->SetCanSwap(TRUE);
		_object->SetColorData(color);
	}
#if BOSS_MODE	
	//攻撃跳ね返すモードの処理
	//赤色のプレイヤーに当たった時の処理
	if (!player_hit && _object->GetObjectType() == PLAYER && _object->GetColorData() == RED)
	{
		player_hit = true;
		this->SetColorData(RED);
	}
#endif
}

bool BossAttackWood::SearchColor(Object* ob)
{
	return false;
}

void BossAttackWood::MoveBamboo()
{
	//location.x += velocity.x;	//上に向かって生えるからX方向いらないかも
	//location.y += velocity.y;

	/*erea.y -= velocity.y;

	if (abs(erea.y) > bambooHeight) {
		velocity = { 0.f,0.f };
	}*/


	//location.x += velocity.x;	//上に向かって生えるからX方向いらないかも
	location.y += velocity.y;
	if (location.y < startLoc.y - erea.y) {
		velocity.y = 0.f;
	}
}
