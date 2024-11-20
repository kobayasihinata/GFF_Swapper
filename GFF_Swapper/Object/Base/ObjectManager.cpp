#include "ObjectManager.h"
#include "../../Scene/GameMain.h"

void ObjectManager::Initialize()
{
	frame = 0;
	camera = Camera::Get();

	effect_spawner = new EffectSpawner();
	effect_spawner->Initialize();

	change_state = GameMainState::Null;
	change_stage = -1;
}

void ObjectManager::Update(GameMain* _g)
{
	frame++;
	//オブジェクト配列に追加する処理
	for (const auto& create_object : create_object)
	{
		if (create_object.stage_around_data != nullptr)
		{
			for (int i = 0; i < 8; i++)
			{
				create_object.object->SetAroundBlock(i, create_object.stage_around_data[i]);
			}
		}
		create_object.object->Initialize(create_object.location, create_object.size, create_object.color, 0);
		object_list.push_back(create_object.object);
	}

	//追加したオブジェクトは消去
	create_object.clear();

	//オブジェクト配列から削除する処理
	for (const auto& delete_object : delete_object)
	{
		for (auto it = object_list.begin(); it != object_list.end();)
		{
			if (*it == delete_object)
			{

				it = object_list.erase(it);
				for (auto it2 = in_screen_object.begin(); it2 != in_screen_object.end();)
				{
					if (*it2 == delete_object)
					{
						it2 = in_screen_object.erase(it2);
						break;
					}
					else
					{
						++it2;
					}
				}
				break;
			}
			else
			{
				++it;
			}
		}
	}

	//削除したオブジェクトは消去
	delete_object.clear();

	//更新処理
	InScreenUpdate();

	//画面内のオブジェクトを更新する処理
	if (!GetSearchFlg() || (GetSearchFlg()&& frame % 10 == 0))
	{
		for (const auto& in_screen_object : in_screen_object)
		{
			in_screen_object->Update(this);
			move_object_num++;
			for (const auto& in_screen_object2 : this->in_screen_object)
			{
				//各オブジェクトとの当たり判定
				if (in_screen_object->HitBox(in_screen_object2) &&
					in_screen_object != in_screen_object2)
				{
					in_screen_object->Hit(in_screen_object2);
				}
			}
			//プレイヤーに選択されているオブジェクトなら、描画色を変える
			if (in_screen_object == now_current_object)
			{
				in_screen_object->SetDrawColor(WHITE);
			}
			else
			{
				in_screen_object->SetDrawColor(in_screen_object->GetColorData());
			}
		}

		//当たり判定処理(いずれ上の当たり判定処理を引っ越す)
		HitCheck();

		//ボスの更新
		if(boss_object != nullptr)BossUpdate();
	}

	//プレイヤー更新
	PlayerUpdate(_g);

	//管理クラスの更新
	effect_spawner->Update(this);

	//暗転フラグの更新	
	boss_blind_flg = _g->GetBlindFlg();

	//ゲームメインステートの変更をする必要があるなら変更
	if (change_state != GameMainState::Null)
	{
		_g->UpdateState(change_state);
		change_state = GameMainState::Null;
	}
	//ステージの変更があるなら変更
	if (change_stage != -1)
	{
		_g->SetStage(change_stage);
		change_stage = -1;
	}
}

void ObjectManager::Draw()const
{
	std::vector<Object*> enemy;
	//オブジェクトの描画
	for (const auto& in_screen_object : in_screen_object)
	{
		if (boss_blind_flg == true)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 - sqrtf(powf(fabsf(player_object->GetLocation().x - in_screen_object->GetLocation().x), 2) + powf(fabsf(player_object->GetLocation().y - in_screen_object->GetLocation().y), 2))));
			in_screen_object->Draw();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		else
		{
			//敵以外なら描画、敵なら配列に格納してまとめて描画
			if (in_screen_object->GetObjectType() != ENEMY)
			{
				in_screen_object->Draw();
			}
			else
			{
				enemy.push_back(in_screen_object);
			}

		}
	}
	//敵描画
	for (const auto& enemy : enemy)
	{
		enemy->Draw();
	}

	//プレイヤーの描画
	player_object->Draw();

	//ボスの描画
	if (!boss_blind_flg)
	{
		if (boss_object != nullptr)boss_object->Draw();
	}

	//エフェクトの描画
	effect_spawner->Draw();

}

void ObjectManager::Finalize()
{
	effect_spawner->Finalize();
	delete effect_spawner;
}

void ObjectManager::CreateObject(Object* _object, Vector2D _location, Vector2D _erea, int _color_data)
{
	SpawnData data = { _object,_location,_erea,_color_data };
	create_object.push_back(data);
}

void ObjectManager::CreateObject(SpawnData _data)
{
	create_object.push_back(_data);
}

void ObjectManager::CreatePlayer(Object* _object, Vector2D _location, Vector2D _erea, int _color_data)
{
	if (player_object != nullptr)
	{
		player_object->Finalize();
		delete player_object;
	}
	player_object = _object;
	player_object->Initialize(_location, _erea, _color_data, 0);
}

void ObjectManager::CreateBoss(Object* _object, Vector2D _location, Vector2D _erea, int _color_data)
{
	boss_object = _object;
	boss_object->Initialize(_location, _erea, _color_data, 0);
}

void ObjectManager::DeleteObject(Object* _object)
{
	delete_object.push_back(_object);
}

void ObjectManager::DeleteAllObject(bool _player_delete)
{
	//完全消去
	create_object.clear();
	in_screen_object.clear();
	delete_object.clear();
	object_list.clear();

	if (boss_object != nullptr)
	{
		boss_object = nullptr;
	}

	if (_player_delete)
	{
		player_object = nullptr;
	}
}

void ObjectManager::HitCheck()
{

}

void ObjectManager::InScreenUpdate()
{
	for (const auto& object_list : object_list)
	{
		//ローカル座標の更新
		object_list->SetScreenPosition(camera->GetCameraLocation());

		//既にスクリーン内に入っているオブジェクトなら、追加処理は省略する
		auto result = std::find(in_screen_object.begin(), in_screen_object.end(), object_list);
		if (result == in_screen_object.end())
		{
			//スクリーン内に入っているオブジェクトを追加する
			if (CheckInScreen(object_list))
			{
				in_screen_object.push_back(object_list);
			}
		}
	}
	//画面外に出たオブジェクトを配列から除外する
	for (auto it = in_screen_object.begin(); it != in_screen_object.end();)
	{
		if (!CheckInScreen(*it))
		{
			it = in_screen_object.erase(it);
		}
		else
		{
			++it;
		}
	}
}

bool ObjectManager::CheckInScreen(Object* _object)const
{
	//画面内に居るか判断 画面端の敵がすり抜けないように地面の更新範囲を広めに
	if (_object != nullptr &&
		(
			(_object->GetObjectType() != ENEMY &&
				_object->GetLocation().x > camera->GetCameraLocation().x - _object->GetErea().x - 80 &&
				_object->GetLocation().x < camera->GetCameraLocation().x + SCREEN_WIDTH + _object->GetErea().x + 80 &&
				_object->GetLocation().y > camera->GetCameraLocation().y - _object->GetErea().y - 80 &&
				_object->GetLocation().y < camera->GetCameraLocation().y + SCREEN_HEIGHT + _object->GetErea().y + 160
				)
			||
			(_object->GetObjectType() == ENEMY &&
				_object->GetLocation().x > camera->GetCameraLocation().x - _object->GetErea().x &&
				_object->GetLocation().x < camera->GetCameraLocation().x + SCREEN_WIDTH + _object->GetErea().x &&
				_object->GetLocation().y > camera->GetCameraLocation().y - _object->GetErea().y &&
				_object->GetLocation().y < camera->GetCameraLocation().y + SCREEN_HEIGHT + _object->GetErea().y
				)
			)
		)
	{
		return true;
	}
	return false;
}

void ObjectManager::PlayerUpdate(GameMain* _g)
{

	//プレイヤーが居ないなら(DeleteObjectされていた、もしくはobject[player_object]がプレイヤーではないなら)
	if (player_object == nullptr || player_object->GetObjectType() != PLAYER)
	{
		//プレイヤーの生成
		CreateObject(new Player, Vector2D(100,100), {PLAYER_HEIGHT,PLAYER_WIDTH}, GREEN);
	}

	//プレイヤーの更新＆色探知用
	if (player_object != nullptr)
	{
		player_object->SetScreenPosition(camera->GetCameraLocation());
		player_object->Update(this);
		move_object_num++;

		for (const auto& in_screen_object : in_screen_object)
		{
			if (in_screen_object->GetCanSwap() == TRUE && in_screen_object->GetObjectType() != PLAYER && boss_blind_flg == false) {
				player_object->SearchColor(in_screen_object);
			}

			//各オブジェクトとの当たり判定
			if (in_screen_object->HitBox(player_object))
			{
				in_screen_object->Hit(player_object);
				player_object->Hit(in_screen_object);
			}
		}

		//ボスを色交換対象にする
		if (boss_object != nullptr)
		{
			if (boss_object->GetCanSwap() == TRUE && boss_object->GetObjectType() != PLAYER && boss_blind_flg == false) {
				player_object->SearchColor(boss_object);
			}
		}
		//プレイヤーが落下したときに死亡判定とする
		if (player_object->GetLocation().y > camera->GetStageSize().y + 100)
		{
			//ステージとプレイヤーをリセット
			//SetStage(now_stage, true);

			change_state = GameMainState::GameOver;
		}
	}
}

void ObjectManager::BossUpdate()
{
	if (boss_object != nullptr)
	{
		boss_object->SetScreenPosition(camera->GetCameraLocation());
		boss_object->Update(this);
		move_object_num++;
		for (const auto& in_screen_object : in_screen_object)
		{
			//各オブジェクトとの当たり判定
			if (in_screen_object->HitBox(boss_object))
			{
				in_screen_object->Hit(boss_object);
				boss_object->Hit(in_screen_object);
			}
		}
	}
}

void ObjectManager::SetNowCurrentObject(Object* _object)
{
	now_current_object = _object;
}

bool ObjectManager::GetSearchFlg()const
{
	return player_object->GetSearchFlg();
}

Vector2D ObjectManager::GetPlayerLocation()const
{
	return player_object->GetLocation();
}

Vector2D ObjectManager::GetPlayerLocalLocation()const
{
	return player_object->GetLocalLocation();
}

Vector2D ObjectManager::GetPlayerErea()const
{
	return player_object->GetErea();
}

int ObjectManager::GetPlayerColor()const
{
	return player_object->GetColorData(); 
}

Vector2D ObjectManager::GetBossLocation()const
{
	return boss_object->GetLocation();
}

void ObjectManager::UpdateState(GameMainState _state)
{
	change_state = _state;
}

void ObjectManager::SpawnEffect(Vector2D _location, Vector2D _erea, int _type, int _time, int _color, float _angle)
{
	effect_spawner->SpawnEffect(_location, _erea, _type, _time, _color, _angle);
}

int ObjectManager::Swap(Object* _object1, Object* _object2)
{
	return effect_spawner->Swap(_object1, _object2);
}