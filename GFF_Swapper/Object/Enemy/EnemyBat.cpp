#include"EnemyBat.h"
#include"../../Utility/KeyInput.h"
#include "../../Utility/ResourceManager.h"
#include<math.h>

#include "../../Utility/DebugInformation.h"


#define _PI 3.141592654f

#define ENEMY_SPEED 2.f

EnemyBat::EnemyBat() :up(0), bat_state(BatState::LEFT), wing_angle(0.0f), vector{ 0.0f }, faint_timer(0), death_timer(0), se_once(false),bat_state_num(0)
{
	object_type = ENEMY;
	can_swap = TRUE;
	can_hit = TRUE;
	for (int i = 0; i < 4; i++){
		move[i] = 0;
	}
	for (int i = 0; i < 4; i++) {
		stageHitFlg[1][i] = false;
	}
}

EnemyBat::~EnemyBat()
{
}

void EnemyBat::Initialize(Vector2D _location, Vector2D _erea, int _color_data)
{
	camera = Camera::Get();
	location = { _location };//x座標 ,y座標 //590 ,400
	erea = { _erea };		//高さ、幅	//100,150
	color = _color_data;

	vector = ENEMY_SPEED;

	bat_image = ResourceManager::SetDivGraph("Resource/Images/sozai/bat_fly.PNG", 12, 4, 3, 145, 107, 5, true);
	damage_image = ResourceManager::SetDivGraph("Resource/Images/sozai/bat_damage.PNG", 3, 3, 1, 145, 107, 0, false);

	wing_se = ResourceManager::SetSound("Resource/Sounds/Enemy/flapping_wings.wav");
	damage_se[0] = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_damage_fire.wav");
	damage_se[1] = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_damage_grass.wav");
	damage_se[2] = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_damage_water.wav");
	faint_se = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_faint.wav");
	fall_se = ResourceManager::SetSound("Resource/Sounds/Player/player_fall.wav");
	hit_se = ResourceManager::SetSound("Resource/Sounds/System/hit.wav");

}

void EnemyBat::Update(ObjectManager* _manager)
{
	__super::Update(_manager);


	up += 1;
	// 羽の角度を変化させる
	if (bat_state != BatState::DEATH)
	{
		wing_angle = (float)sin(PI * 2.f / 40.f * up) * 20.f; // 30度の振れ幅で周期的に変化させる
	}
	if (wing_angle > 19)
	{
		if (se_once == false)
		{
			ResourceManager::StartSound(wing_se);
			se_once = true;
		}
	}
	else
	{
		se_once = false;
	}

	//スタン状態でないなら各種処理を実行
	if (bat_state != BatState::FAINT)
	{
		Vector2D player_pos = _manager->GetPlayerLocation();
		Vector2D player_erea = _manager->GetPlayerErea();
		// プレイヤーとの距離を計算

		// プレイヤーの中心座標を計算
		float player_center_x = player_pos.x + player_erea.x / 2;
		float player_center_y = player_pos.y + player_erea.y / 2;

		// 自分の中心座標を計算
		float enemy_center_x = location.x + erea.x / 2;
		float enemy_center_y = location.y + erea.y / 2;

		// プレイヤーの中心座標との距離を計算
		float dx = player_center_x - enemy_center_x + 10;
		float dy = player_center_y - enemy_center_y + 10;
		float length = sqrtf(dx * dx + dy * dy);

		//プレイヤーの一定範囲内に入ったら
		if (length < 450 && bat_state != BatState::DEATH && !wall_hit_flg) {
			// 移動方向を決定
			dx /= length;
			dy /= length;
			
			len = dx;
			if (dx > 0)
			{
				bat_state = BatState::RIGHT;
				//vector.x = ENEMY_SPEED;
			}
			else
			{
				bat_state = BatState::LEFT;
				//vector.x = -ENEMY_SPEED;
			}

			// 移動する
			//location.x += vector.x;
			location.x += dx * (vector.x);
			location.y += dy * (vector.y);

			//加速度を敵の基本速度に近くなるように加算、減算していく
			if (vector.x > ENEMY_SPEED)vector.x--;
			if (vector.x < ENEMY_SPEED)vector.x++;
			if (vector.y > ENEMY_SPEED)vector.y--;
			if (vector.y < ENEMY_SPEED)vector.y++;

			wall_hit_timer = 0;
		}
		else
		{
			//移動
			Move(_manager);
		}
	}
	//スタン状態なら時間測定
	else
	{
		if (++faint_timer > FAINT_TIME)
		{
			faint_timer = 0;
			bat_state = BatState::LEFT;
		}

		location += vector;
		//加速度を敵の基本速度に近くなるように加算、減算していく
		if (vector.x > 0)vector.x--;
		if (vector.x < 0)vector.x++;
		if (vector.y > 0)vector.y--;
		if (vector.y < 0)vector.y++;
	}
	
	//落下死処理
	if (location.y > camera->GetStageSize().y)
	{
		bat_state = BatState::DEATH;
		ResourceManager::StartSound(fall_se);
	}
	for (int i = 0; i < 4; i++) {
		stageHitFlg[0][i] = false;
		stageHitFlg[1][i] = false;
	}

	if (wall_hit_flg)
	{
		wall_hit_timer++;
		if (wall_hit_timer > 60)
		{
			wall_hit_timer = 0;
			wall_hit_flg = false;
		}
	}

}

void EnemyBat::Draw() const
{
	//DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, GetColor(255, 255, 255), FALSE);

	//各頂点をlocal_locationに置き換えた

	DebugInformation::Add("bat_state", len);
	DebugInformation::Add("time", wall_hit_timer);


	const std::vector<Vector2D> vertices = {
		// 耳
		{local_location.x + 46, local_location.y}, {local_location.x + 46, local_location.y + 19}, {local_location.x + 55, local_location.y + 9},
		{local_location.x + 69, local_location.y}, {local_location.x + 69, local_location.y + 19}, {local_location.x + 60, local_location.y + 9},
		//右翼
		{local_location.x + 66, local_location.y + 33}, {local_location.x + 97, local_location.y + 7}, {local_location.x + 117, local_location.y + 65},
		{local_location.x + 66, local_location.y + 33}, {local_location.x + 97, local_location.y + 7}, {local_location.x + 96, local_location.y + 61},
		{local_location.x + 65, local_location.y + 33}, {local_location.x + 97, local_location.y + 7}, {local_location.x + 80, local_location.y + 57},
		//左翼
		{local_location.x + 48, local_location.y + 33}, {local_location.x + 20, local_location.y + 7}, {local_location.x - 2, local_location.y + 65},
		{local_location.x + 48, local_location.y + 33}, {local_location.x + 20, local_location.y + 7}, {local_location.x + 18, local_location.y + 61},
		{local_location.x + 50, local_location.y + 33}, {local_location.x + 20, local_location.y + 7}, {local_location.x + 34, local_location.y + 57},
		// 頭
		{local_location.x + 57, local_location.y + 12}, {local_location.x + 69, local_location.y + 23}, {local_location.x + 57, local_location.y + 30}, {local_location.x + 46, local_location.y + 23},
		// 胴体
		{local_location.x + 57, local_location.y + 34}, {local_location.x + 69, local_location.y + 52}, {local_location.x + 57, local_location.y + 73}, {local_location.x + 46, local_location.y + 52},
	};

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (death_timer * 4));

	//スタン状態か死亡状態でないなら描画
	if (bat_state != BatState::FAINT && bat_state != BatState::DEATH)
	{
		if (bat_state == BatState::LEFT)
		{
			ResourceManager::DrawColorAnimGraph(local_location + (erea / 2), bat_image, color, true);
		}
		else
		{
			ResourceManager::DrawColorAnimGraph(local_location + (erea / 2), bat_image, color, false);
		}

		////配列の各頂点を利用して三角形を描画する
		//for (int i = 0; i + 2 < vertices.size(); i += 3) {
		//	//耳
		//	if (i < 5) {
		//		DrawTriangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, color, TRUE);
		//		DrawTriangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, 0x000000, FALSE);
		//	}
		//	//右羽
		//	else if (i >= 6 && i < 14) {
		//		// 羽の動き
		//		DrawLineAA(vertices[7].x, vertices[7].y + wing_angle, vertices[8].x + wing_angle, vertices[8].y, 0x000000);
		//		DrawLineAA(vertices[6].x, vertices[6].y - 2, vertices[7].x, vertices[7].y - 2 + wing_angle, 0x000000);
		//		//DrawTriangleAA(vertices[i].x - 1, vertices[i].y - 1, vertices[i + 1].x - 1, vertices[i + 1].y - 1 + wing_angle, vertices[i + 2].x + wing_angle - 1, vertices[i + 2].y - 1, 0x000000, FALSE);
		//		DrawTriangleAA(verをtices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y + wing_angle, vertices[i + 2].x + wing_angle, vertices[i + 2].y, color, TRUE);

		//	}
		//	//左羽
		//	else if (i >= 15 && i < 23) {
		//		// 羽の動き
		//		DrawLineAA(vertices[16].x, vertices[16].y - 2 + wing_angle, vertices[17].x - wing_angle, vertices[17].y, 0x000000);
		//		DrawLineAA(vertices[15].x, vertices[15].y - 2, vertices[16].x, vertices[16].y - 2 + wing_angle, 0x000000);
		//		//DrawTriangleAA(vertices[i].x + 1, vertices[i].y + 1, vertices[i + 1].x + 1, vertices[i + 1].y - 1 + wing_angle, vertices[i + 2].x - 3 - wing_angle, vertices[i + 2].y - 3, 0x000000, FALSE);
		//		DrawTriangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y + wing_angle, vertices[i + 2].x - wing_angle, vertices[i + 2].y, color, TRUE);

		//	}
		//	//ひし形の描画
		//	else if (i + 3 < vertices.size())
		//	{
		//		DrawQuadrangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, vertices[i + 3].x, vertices[i + 3].y, color, TRUE);
		//		DrawQuadrangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, vertices[i + 3].x, vertices[i + 3].y, 0x000000, FALSE);
		//		i++;
		//	}
		//}
	}
	//スタンか死亡ならダメージ画像を描画
	else
	{
		//点滅
		if (frame % 3 != 0)
		{
			if (len < 0) {
				DrawRotaGraphF(local_location.x + (erea.x / 2),
					local_location.y + (erea.y / 2),
					1.0f,
					0,
					ResourceManager::GetDivGraph(damage_image, GetColorNum(color)),
					TRUE,
					true);
			}
			else
			{
				DrawRotaGraphF(local_location.x + (erea.x / 2),
					local_location.y + (erea.y / 2),
					1.0f,
					0,
					ResourceManager::GetDivGraph(damage_image, GetColorNum(color)),
					TRUE,
					false);
			}
			
		}
	}
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	/*
	if (hit_flg[0]) {
		DrawString(200, 200, "hit", 0xfffff);
	}*/
}

void EnemyBat::Finalize()
{

}

void EnemyBat::Move(ObjectManager* _manager)
{
	//加速度を固定
	//vector.x = ENEMY_SPEED;

	switch (bat_state)
	{
	case BatState::IDLE:
		break;
	case BatState::LEFT:
		vector.x = -ENEMY_SPEED;
		//location.x -= vector.x;
		location.y += (float)sin(PI * 2.f / 40.f * up) * 5.f;
		bat_state_num = 1;
		break;
	case BatState::RIGHT:
		vector.x = ENEMY_SPEED;
		//location.x += vector.x;
		location.y += (float)sin(PI * 2.f / 40.f * up) * 5.f;
		bat_state_num = 2;
		break;
	case BatState::FAINT:
		bat_state_num = 3;
		break;
	case BatState::DEATH:
		//自分の色が青のとき吸われてく
		bat_state_num = 4;
		if (vector.x > 0) {
			vector.x = 0.2f;
		}
		else {
			vector.x = -0.2f;
		}
		if (++death_timer > 60) {
			if (this != nullptr) {
				_manager->DeleteObject(this);
			}
		}
		break;
	default:
		break;
	}

	location.x += vector.x;
	////左移動
	//if (bat_state == BatState::LEFT) {
	//	location.x -= vector.x;
	//	location.y += (float)sin(PI * 2.f / 40.f * up) * 5.f;
	//	bat_state_num = 1;

	//}
	////右移動
	//if (bat_state == BatState::RIGHT) {
	//	location.x += vector.x;
	//	location.y += (float)sin(PI * 2.f / 40.f * up) * 5.f;
	//	bat_state_num = 2;
	//}

	//if (bat_state == BatState::DEATH) {
	//	//自分の色が青のとき吸われてく
	//	bat_state_num = 4;

	//	if (++death_timer > 60) {
	//		if (this != nullptr) {
	//			_manager->DeleteObject(this);
	//		}
	//	}
	//	
	//	/*else {
	//		if (this != nullptr) {
	//			_manager->DeleteObject(this);
	//		}
	//	}*/
	//}
}

void EnemyBat::Hit(Object* _object)
{
	/*delete_object = _object;*/
	//ブロックと当たった時の処理
	if (_object->object_type == BLOCK || 
		_object->object_type == GROUND_BLOCK|| 
		(_object->can_swap == TRUE && CheckCompatibility(this, _object) == 0)
		)
		
	{
		Vector2D tmpl = location;
		Vector2D tmpe = erea;
		move[0] = 0;
		move[1] = 0;
		move[2] = 0;
		move[3] = 0;

		//上下判定用に座標とエリアの調整
		location.x += 10.f;
		erea.y = 1.f;
		erea.x = tmpe.x - 15.f;

		//プレイヤー上方向の判定
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][top] && bat_state != BatState::DEATH) {
			stageHitFlg[0][top] = true;
			stageHitFlg[1][top] = true;
		}
		else {
			stageHitFlg[0][top] = false;
		}

		//プレイヤー下方向の判定
		location.y += tmpe.y + 1;
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][bottom] && bat_state != BatState::DEATH) {
			stageHitFlg[0][bottom] = true;
			stageHitFlg[1][bottom] = true;
		}
		else {
			stageHitFlg[0][bottom] = false;
		}

		//戻す
		location.x = tmpl.x;
		location.y = tmpl.y;
		erea.y = tmpe.y;
		erea.x = tmpe.x;

		//上方向に埋まらないようにする
		if (stageHitFlg[0][top]) {//上方向に埋まっていたら
			float t = (_object->GetLocation().y + _object->GetErea().y) - location.y;
			if (t != 0) {
				vector.y = 0.f;
				move[top] = t;

			}
		}

		//下方向に埋まらないようにする
		if (stageHitFlg[0][bottom]) {//下方向に埋まっていたら
			float t = _object->GetLocation().y - (location.y + erea.y);
			if (t != 0) {
				move[bottom] = t;
			}
		}


		//左右判定用に座標とエリアの調整
		location.y += 3.f;
		erea.y = tmpe.y - 3.f;
		erea.x = 1;

		//プレイヤー左方向の判定
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][left] && bat_state != BatState::DEATH) {
			stageHitFlg[0][left] = true;
			stageHitFlg[1][left] = true;
			int a = CheckCollision(_object->GetLocation(), _object->GetErea());
		}
		else {
			stageHitFlg[0][left] = false;
		}


		//プレイヤー右方向の判定
		location.x = tmpl.x + tmpe.x + 1;
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][right] && bat_state != BatState::DEATH) {
			stageHitFlg[0][right] = true;
			stageHitFlg[1][right] = true;
		}
		else {
			stageHitFlg[0][right] = false;
		}

		//最初の値に戻す

		location.x = tmpl.x;
		location.y += -3.f;
		erea.y = tmpe.y;
		erea.x = tmpe.x;



		//左方向に埋まらないようにする
		if (stageHitFlg[0][left]) {//左方向に埋まっていたら
			float t = (_object->GetLocation().x + _object->GetErea().x) - location.x;
			if (t != 0) {
				vector.x = 0.f;
				move[left] = t;
				if (bat_state != BatState::FAINT && _object->object_type != PLAYER)
				{
					bat_state = BatState::RIGHT;
					wall_hit_flg = true;
				}
			}
		}

		//右方向に埋まらないようにする
		if (stageHitFlg[0][right]) {//右方向に埋まっていたら
			float t = _object->GetLocation().x - (location.x + erea.x);
			if (t != 0) {
				vector.x = 0.f;
				move[right] = t;
				if (bat_state != BatState::FAINT && _object->object_type != PLAYER)
				{
					bat_state = BatState::LEFT;
					wall_hit_flg = true;
				}
			}
		}


		//上下左右の移動量から移動後も埋まってるか調べる
		if (location.y < _object->GetLocation().y + _object->GetErea().y && location.y + erea.y > _object->GetLocation().y) {//左右
			if (stageHitFlg[1][top] || stageHitFlg[1][bottom]) {
				move[left] = 0.f;
				move[right] = 0.f;
			}
		}

		location.x += move[left];
		location.x += move[right];
		location.y += move[top];
		location.y += move[bottom];

		erea.y = tmpe.y;
		erea.x = tmpe.x;

	}
	//赤コウモリ
	//触れたブロックが緑＆自分の色が赤だったら触れた緑ブロックを燃やす
	//水の中に突っ込むと即死
	if (_object->GetColorData() == BLUE &&
		_object->object_type != PLAYER &&
		this->color == RED) {
		
		//死亡状態へ
		if (bat_state != BatState::DEATH)
		{
			bat_state = BatState::DEATH;
			ResourceManager::StartSound(damage_se[_object->object_type - 3]);
			can_swap = FALSE;
		}
	}

	//青コウモリ
	//触れたブロックが赤＆自分の色が青だったら触れた赤ブロックを消す
	//コウモリの色が吸い取られて死ぬ
	if (_object->GetColorData() == GREEN &&
		_object->object_type != PLAYER &&
		this->color == BLUE) {
		//wing_angle = sin(PI * 2.f / 12.f * up) * 20.f; // 藻掻いているように見える風に
		//死亡状態へ
		if (bat_state != BatState::DEATH)
		{
			bat_state = BatState::DEATH;
			ResourceManager::StartSound(damage_se[_object->object_type - 3]);
			can_swap = FALSE;
		}
	}

	//緑コウモリ
	//触れたブロックが青＆自分の色が緑だったら、雨粒を吸い取り　水場などに当たると反射する
	//当たったら即死
	if (_object->GetColorData() == RED &&
		_object->object_type != PLAYER &&
		this->color == GREEN) {
		if (bat_state != BatState::DEATH)
		{
			bat_state = BatState::DEATH;
			ResourceManager::StartSound(damage_se[_object->object_type - 3]);
			can_swap = FALSE;
		}
	}

	//ダメージゾーンを上書きする
	if (!_object->can_swap && !_object->is_boss_attack&&
		((this->color == GREEN && _object->object_type == WATER) ||
		(this->color == BLUE && _object->object_type == FIRE) ||
		(this->color == RED && _object->object_type == WOOD)))
	{
		_object->SetColorData(color);
	}

	//プレイヤーと当たった時の処理
	if (_object->object_type == PLAYER)
	{
		//プレイヤーとの属性相性で処理を変える
		switch (CheckCompatibility(this, _object))
		{
			//不利の場合
		case -1:
			//スタン状態か死亡状態でないなら実行
			if (bat_state != BatState::FAINT && bat_state != BatState::DEATH)
			{
				//プレイヤーが左にいるなら右にノックバック
				if (this->location.x > _object->GetLocation().x)
				{
					vector.x = 20;
				}
				//プレイヤーが右にいるなら左にノックバック
				else
				{
					vector.x = -20;
				}
				//プレイヤーが上にいるなら下にノックバック
				if (this->location.y > _object->GetLocation().y)
				{
					vector.y = 20;
				}
				//プレイヤーが下にいるなら上にノックバック
				else
				{
					vector.y = -20;
				}
				//自身をスタン状態にする
				bat_state = BatState::FAINT;
				//スタンSE再生
				ResourceManager::StartSound(faint_se);
				//ぶつかるSE再生
				ResourceManager::StartSound(hit_se);
			}
			break;
			//あいこの場合
		case 0:
			if(bat_state != BatState::DEATH)
			{ 
				vector.x = -10;
				vector.y = 10;
			}
			//ぶつかるSE再生
			ResourceManager::StartSound(hit_se);
			break;
			//有利の場合
		case 1:
			break;
			//それ以外
		default:
			break;
		}
	}

	//エネミーと当たった時の処理
	if (_object->object_type == ENEMY)
	{
		//エネミーとの属性相性で処理を変える
		switch (CheckCompatibility(this, _object))
		{
			//不利の場合
		case -1:
			//何もしない
			break;

			//あいこの場合
		case 0:
			//プレイヤーが左にいるなら右にノックバック
			if (this->location.x > _object->GetLocation().x)
			{
				vector.x = 20;
			}
			//プレイヤーが右にいるなら左にノックバック
			else
			{
				vector.x = -20;
			}
			//プレイヤーが上にいるなら下にノックバック
			if (this->location.y > _object->GetLocation().y)
			{
				vector.y = 20;
			}
			//プレイヤーが下にいるなら上にノックバック
			else
			{
				vector.y = -20;
			}
			break;
			//有利の場合
		case 1:
			//何もしない
			break;

			//それ以外
		default:
			break;
		}
	}
}

bool EnemyBat::CheckCollision(Vector2D l, Vector2D e)
{
	bool ret = false;

	//自分の左上座標
	float my_x = location.x;
	float my_y = location.y;
	//自分の中央座標
	float my_cx = my_x + (erea.x / 2);
	float my_cy = my_y + (erea.y / 2);
	//自分の幅と高さの半分
	float my_harf_width = erea.x / 2;
	float my_harf_height = erea.y / 2;

	//相手の左上座標
	float sub_x = l.x;
	float sub_y = l.y;
	//相手の中央座標
	float sub_cx = sub_x + (e.x / 2);
	float sub_cy = sub_y + (e.y / 2);
	//相手の幅と高さの半分
	float sub_harf_width = e.x / 2;
	float sub_harf_height = e.y / 2;

	//自分と相手の中心座標の差
	float diff_x = my_cx - sub_cx;
	float diff_y = my_cy - sub_cy;

	//当たり判定の演算
	if (fabsf(diff_x) < my_harf_width + sub_harf_width &&
		fabsf(diff_y) < my_harf_height + sub_harf_height)
	{
		ret = true;
	}
	return ret;
}
