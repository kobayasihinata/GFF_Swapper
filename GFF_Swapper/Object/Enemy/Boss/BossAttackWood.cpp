#include "BossAttackWood.h"
#include "../../../Scene/GameMain.h"
#include "../../../Utility/ResourceManager.h"

BossAttackWood::BossAttackWood()
{
	camera = Camera::Get();
	start_location_y = 0;
	velocity = { 0.f,0.f };
	start_loc = { 0.f,0.f };

	spawn_count = 0;
	delete_count = 0;
	w_type = 0;
	camera_impact_once = false;
	start_se = 0;

	object_type = WOOD;
	can_swap = FALSE;
	can_hit = FALSE;
	is_boss_attack = TRUE;
}

BossAttackWood::~BossAttackWood()
{
}

void BossAttackWood::Initialize(Vector2D _location, Vector2D _erea, int _color_data)
{
	location = _location;
	start_location_y = location.y;

	color = _color_data;

	velocity.x = 0.f;
	velocity.y = -10.f;	//上向きに生える為

	erea.y = _erea.x;
	erea.x = _erea.y;

	start_loc = _location;

	start_se = ResourceManager::SetSound("Resource/Sounds/Enemy/Boss/attack_wood.wav");

}

void BossAttackWood::Finalize()
{
}

void BossAttackWood::Update(ObjectManager* _manager)
{
	__super::Update(_manager);

	//一定時間待ってから移動
	if (++spawn_count > B_WOOD_SPAWN_TIME)
	{
		MoveBamboo();
	}
	
	//移動後処理
	if (velocity.y == 0.f) {
		//一回だけカメラ振動
		if (camera_impact_once == false)
		{
			camera->SetImpact(15);
			ResourceManager::StartSound(start_se);
			camera_impact_once = true;
		}
		//一定時間経過で自身を消去
		if (++delete_count > B_WOOD_DELETE_TIME) {
			_manager->DeleteObject(this);
		}
	}
}

void BossAttackWood::Draw() const
{
	//警告表示
	if (spawn_count < B_WOOD_SPAWN_TIME && spawn_count % 4 != 0)
	{
		DrawBoxAA(local_location.x, SCREEN_HEIGHT - 40, local_location.x + erea.x, SCREEN_HEIGHT, 0x00ff00, true);
		DrawBoxAA(local_location.x+17, SCREEN_HEIGHT - 37, local_location.x + erea.x-17, SCREEN_HEIGHT-17, 0x000000, true);
		DrawBoxAA(local_location.x+17, SCREEN_HEIGHT - 15, local_location.x + erea.x-17, SCREEN_HEIGHT-5, 0x000000, true);
		DrawBoxAA(local_location.x, 0, local_location.x + erea.x, SCREEN_HEIGHT, 0x00ff00, false);
	}
	//竹表示
	for (int i = 0; i < (erea.y / BOX_HEIGHT); i++)
	{
		//キャノンの画像より上の竹しか描画しない
		if (start_location_y > location.y + (i * BOX_HEIGHT)+ BOX_HEIGHT)
		{
			//ResourceManager::StageAnimDraw({ local_location.x, local_location.y + (i * BOX_HEIGHT) }, WOOD);
			DrawBoxAA(local_location.x + 3, 
					  local_location.y + (i * BOX_HEIGHT), 
					  local_location.x + erea.x - 3, 
					  local_location.y + (i * BOX_HEIGHT)+BOX_HEIGHT, 0x00cc00, true);

			DrawBoxAA(local_location.x + 2, local_location.y + (i * BOX_HEIGHT), local_location.x + erea.x - 2, local_location.y + 2 + (i * BOX_HEIGHT), 0x00ff00, true);
			DrawBoxAA(local_location.x + 2, local_location.y+ (i * BOX_HEIGHT), local_location.x + erea.x - 2, local_location.y  - 2 + (i * BOX_HEIGHT) + BOX_HEIGHT, 0x00ff00, true);
			DrawBoxAA(local_location.x + 10, local_location.y + 2 + (i * BOX_HEIGHT), local_location.x + 13, local_location.y - 2 + (i * BOX_HEIGHT) + BOX_HEIGHT, 0x00ee00, true);
		}
	}
}

void BossAttackWood::Hit(Object* _object)
{
	//他のボス攻撃には干渉しない
	if (!_object->is_boss_attack)
	{
		//水ダメージゾーンに当たったら色だけ変える
		if (_object->object_type == WATER &&
			!_object->can_swap)
		{
			_object->SetColorData(color);
			return;
		}
		//地面か水ブロックを木ブロックに変更
		if ((_object->object_type == GROUND_BLOCK || _object->object_type == WATER) &&
			_object->GetColorData() != WHITE &&
			_object->GetLocation().y < start_location_y) {

			_object->can_swap = TRUE;
			_object->SetColorData(color);
		}
	}
}

bool BossAttackWood::SearchColor(Object* ob)
{
	return false;
}

void BossAttackWood::MoveBamboo()
{
	location.y += velocity.y;
	//竹が伸びきったら停止
	if (location.y < start_loc.y - erea.y) {
		velocity.y = 0.f;
	}
}
