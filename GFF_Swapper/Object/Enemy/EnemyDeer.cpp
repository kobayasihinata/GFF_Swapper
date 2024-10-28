#include "../../Utility/KeyInput.h"
#include "../../Utility/ResourceManager.h"
#include "../../Scene/GameMain.h"
#include "EnemyDeer.h"
#include<math.h>

EnemyDeer::EnemyDeer()
{
	type = ENEMY;
	can_swap = TRUE;
	can_hit = TRUE;

	//deer_state = DeerState::LEFT;
	deer_state = DeerState::GRAVITY;

	//location == 当たり判定用 local_location == 画像を動かすだけの物
	//locationの中に値を入れると 上記の変数のどちらでも値を使用できる
	// { x , y }
	//location = { 500, 850 };

	for (int i = 0; i < 4; i++)
	{
		move[i] = 0;
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			stageHitFlg[i][j] = false;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		d_leg_rad[i] = 0.0f;
		d_left_leg[i] = 0.0f;
		left_Anim_Flg[i] = false;
	}

	deer_spawn = false;
	deer_draw = d_draw::vr_one;

	frame = 0;

	anim_fps = 0;

	for (int i = 0; i < 4; i++) {
		leg_angle[i] = 0.0f;
		anim_flg[i] = false;
	}
	speed = 1.0f;
}

EnemyDeer::~EnemyDeer()
{

}

void EnemyDeer::Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)
{
	//一旦引数はパス 鹿がどこにでるかの座標 
	//地面と完全に座標が一致していると地面に引っかかって動かなくなる 859
	location = _location;
	//当たり判定の大きさを変更できる
	//erea = {100, 100};
	erea = _erea;
	color = _color_data;

	object_pos = _object_pos;

	walk_se = ResourceManager::SetSound("Resource/Sounds/Player/walk_normal.wav");
	damage_se[0] = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_damage_fire.wav");
	damage_se[1] = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_damage_grass.wav");
	damage_se[2] = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_damage_water.wav");
}

void EnemyDeer::Update(GameMain* _g)
{
	++anim_fps;
	__super::Update(_g);
	
	if (frame % 30 == 0)
	{
		ResourceManager::StartSound(walk_se);
	}
	for (int i = 0; i < 4; i++) {
		stageHitFlg[0][i] = false;
		stageHitFlg[1][i] = false;
	}

	/*
		2D 長方形の回転
		r: 角度(ラジアン)
		x : 元のX座標
		y : 元のY座標

		X = x * cos(r) - y * sin(r)
		Y = x * sin(r) + y * cos(r)
	*/

	EnemyDeerMove();

	if (d_leg_rad[0] < 30.f)
	{
		d_leg_rad[0]++;

	}
	/*else
	{
		d_leg_rad[0]--;
	}*/

	if (deer_state == DeerState::DEATH)
	{
		if (++deer_death_timer > 60)
		{
			if (this != nullptr) {
				_g->DeleteObject(object_pos, this);
			}
		}
	}
}

void EnemyDeer::Draw()const
{
	if (deer_state == DeerState::LEFT || deer_state == DeerState::GRAVITY || deer_state == DeerState::IDLE || (deer_state == DeerState::DEATH))
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (deer_death_timer * 4));

		//頭
		ResourceManager::DrawRotaBox(local_location.x + 16.0f, local_location.y + 10.0f, 30.0f, 20.0f, local_location.x + 16.0f, local_location.y + 10.0f, d_head_rad, draw_color, true);
		ResourceManager::DrawRotaBox(local_location.x + 16.0f, local_location.y + 10.0f, 30.0f, 20.0f, local_location.x + 16.0f, local_location.y + 10.0f, d_head_rad, 0x000000, FALSE);
		//目
		ResourceManager::DrawRotaBox(local_location.x + 8.0f, local_location.y + 10.0f, 8.0f, 9.0f, local_location.x + 8.0f, local_location.y + 10.0f, d_eye_rad, 0x000000, true);

		switch (deer_draw)
		{
		case d_draw::vr_one:

			//首 vr.1
			//ResourceManager::DrawRotaBox(local_location.x + 22.0f, local_location.y + 30.0f, 17.0f, 10.0f, local_location.x + 22.0f, local_location.y + 30.0f, d_neck_rad, color, true);
			//ResourceManager::DrawRotaBox(local_location.x + 26.0f, local_location.y + 45.0f, 10.0f, 10.0f, local_location.x + 26.0f, local_location.y + 45.0f, d_neck_rad, color, true);

			ResourceManager::DrawRotaBox(local_location.x + 25.0f, local_location.y + 38.0f, 13.0f, 24.0f, local_location.x + 25.0f, local_location.y + 38.0f, d_neck_rad, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 25.0f, local_location.y + 38.0f, 13.0f, 24.0f, local_location.x + 25.0f, local_location.y + 38.0f, d_neck_rad, 0x000000, FALSE);

			//胴体 vr.1
			ResourceManager::DrawRotaBox(local_location.x + 53.0f, local_location.y + 63.0f, 65.0f, 15.0f, local_location.x + 53.0f, local_location.y + 63.0f, 0.f, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 53.0f, local_location.y + 63.0f, 65.0f, 15.0f, local_location.x + 53.0f, local_location.y + 63.0f, 0.f, 0x000000, FALSE);
			break;

		case d_draw::vr_two:

			//首 vr.2
			ResourceManager::DrawRotaBox(local_location.x + 25.0f, local_location.y + 38.0f, 13.0f, 24.0f, local_location.x + 25.0f, local_location.y + 38.0f, d_neck_rad, draw_color, true);

			//胴体 vr.2
			ResourceManager::DrawRotaBox(local_location.x + 34.0f, local_location.y + 63.0f, 30.0f, 15.0f, local_location.x + 34.0f, local_location.y + 63.0f, d_rad, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 72.0f, local_location.y + 63.0f, 30.0f, 15.0f, local_location.x + 72.0f, local_location.y + 63.0f, d_rad, draw_color, true);
			break;
		}

		//足　左から
		ResourceManager::DrawRotaBox(local_location.x + 27.0f + d_left_leg[0], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 27.0f - d_left_leg[0], local_location.y + 88.0f, leg_angle[0], draw_color, true);
		ResourceManager::DrawRotaBox(local_location.x + 27.0f + d_left_leg[0], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 27.0f - d_left_leg[0], local_location.y + 88.0f, leg_angle[0], 0x000000, FALSE);

		ResourceManager::DrawRotaBox(local_location.x + 43.0f + d_left_leg[1], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 43.0f, local_location.y + 88.0f, -leg_angle[1], draw_color, true);
		ResourceManager::DrawRotaBox(local_location.x + 43.0f + d_left_leg[1], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 43.0f, local_location.y + 88.0f, -leg_angle[1], 0x000000, FALSE);
		//ResourceManager::DrawRotaBox(local_location.x + 35.0f + d_left_leg[1], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 35.0f, local_location.y + 88.0f, -leg_angle[1], draw_color, true);

		ResourceManager::DrawRotaBox(local_location.x + 68.0f + d_left_leg[2], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 68.0f, local_location.y + 88.0f, leg_angle[2], draw_color, true);
		ResourceManager::DrawRotaBox(local_location.x + 68.0f + d_left_leg[2], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 68.0f, local_location.y + 88.0f, leg_angle[2], 0x000000, FALSE);

		ResourceManager::DrawRotaBox(local_location.x + 83.0f + d_left_leg[3], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 83.0f, local_location.y + 88.0f, -leg_angle[3], draw_color, true);
		ResourceManager::DrawRotaBox(local_location.x + 83.0f + d_left_leg[3], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 83.0f, local_location.y + 88.0f, -leg_angle[3], 0x000000, FALSE);
		//ResourceManager::DrawRotaBox(local_location.x + 75.0f + d_left_leg[3], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 75.0f, local_location.y + 88.0f, -leg_angle[3], draw_color, true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	else if (deer_state == DeerState::RIGHT || (deer_state == DeerState::DEATH))
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (deer_death_timer * 4));

		//頭
		ResourceManager::DrawRotaBox(local_location.x - erea.x + 15.0f, local_location.y - 10.0f, 30.0f, 20.0f, local_location.x, local_location.y, d_rad, draw_color, true);
		ResourceManager::DrawRotaBox(local_location.x - erea.x + 15.0f, local_location.y - 10.0f, 30.0f, 20.0f, local_location.x, local_location.y, d_rad, 0x000000, FALSE);
		//目
		ResourceManager::DrawRotaBox(local_location.x - erea.x + 8.0f, local_location.y - 10.0f, 8.0f, 9.0f, local_location.x, local_location.y, d_rad, 0x000000, true);

		switch (deer_draw)
		{
		case d_draw::vr_one:

			//首 vr.1
			//ResourceManager::DrawRotaBox(local_location.x - erea.x + 21.0f, local_location.y - 30.0f, 17.0f, 10.0f, local_location.x, local_location.y, d_rad, color, true);
			//ResourceManager::DrawRotaBox(local_location.x - erea.x + 25.0f, local_location.y - 45.0f, 10.0f, 10.0f, local_location.x, local_location.y, d_rad, color, true);

			ResourceManager::DrawRotaBox(local_location.x - erea.x + 23.0f, local_location.y - 38.0f, 13.0f, 24.0f, local_location.x, local_location.y, d_rad, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - erea.x + 23.0f, local_location.y - 38.0f, 13.0f, 24.0f, local_location.x, local_location.y, d_rad, 0x000000, FALSE);

			//胴体 vr.1
			ResourceManager::DrawRotaBox(local_location.x - erea.x + 52.0f, local_location.y - 63.0f, 65.0f, 15.0f, local_location.x, local_location.y, d_rad, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - erea.x + 52.0f, local_location.y - 63.0f, 65.0f, 15.0f, local_location.x, local_location.y, d_rad, 0x000000, FALSE);
			break;

		case d_draw::vr_two:

			//首 vr.2
			ResourceManager::DrawRotaBox(local_location.x - erea.x + 23.0f, local_location.y - 38.0f, 13.0f, 24.0f, local_location.x, local_location.y, d_rad, draw_color, true);

			//胴体 vr.2
			ResourceManager::DrawRotaBox(local_location.x - erea.x + 52.0f, local_location.y - 63.0f, 65.0f, 15.0f, local_location.x, local_location.y, d_rad, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - erea.x + 52.0f, local_location.y - 63.0f, 65.0f, 15.0f, local_location.x, local_location.y, d_rad, draw_color, true);
			break;
		}

		//足　左から
		ResourceManager::DrawRotaBox(local_location.x + 21.0f + d_left_leg[0], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 21.0f - d_left_leg[0], local_location.y + 88.0f, leg_angle[0], draw_color, true);
		ResourceManager::DrawRotaBox(local_location.x + 21.0f + d_left_leg[0], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 21.0f - d_left_leg[0], local_location.y + 88.0f, leg_angle[0], 0x000000, FALSE);

		ResourceManager::DrawRotaBox(local_location.x + 37.0f + d_left_leg[1], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 37.0f, local_location.y + 88.0f, -leg_angle[1], draw_color, true);
		ResourceManager::DrawRotaBox(local_location.x + 37.0f + d_left_leg[1], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 37.0f, local_location.y + 88.0f, -leg_angle[1], 0x000000, FALSE);
		//ResourceManager::DrawRotaBox(local_location.x + 35.0f + d_left_leg[1], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 35.0f, local_location.y + 88.0f, -leg_angle[1], draw_color, true);

		ResourceManager::DrawRotaBox(local_location.x + 62.0f + d_left_leg[2], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 62.0f, local_location.y + 88.0f, leg_angle[2], draw_color, true);
		ResourceManager::DrawRotaBox(local_location.x + 62.0f + d_left_leg[2], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 62.0f, local_location.y + 88.0f, leg_angle[2], 0x000000, FALSE);

		ResourceManager::DrawRotaBox(local_location.x + 77.0f + d_left_leg[3], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 77.f, local_location.y + 88.0f, -leg_angle[3], draw_color, true);
		ResourceManager::DrawRotaBox(local_location.x + 77.0f + d_left_leg[3], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 77.0f, local_location.y + 88.0f, -leg_angle[3], 0x000000, FALSE);
		//ResourceManager::DrawRotaBox(local_location.x + 75.0f + d_left_leg[3], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 75.0f, local_location.y + 88.0f, -leg_angle[3], draw_color, true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

void EnemyDeer::EnemyDeerMove()
{
	//重力(かけ続けないといけない)
	location.y += 10;

	switch (deer_state)
	{
	case DeerState::GRAVITY:

		location.y += 10;

		break;

	case DeerState::IDLE:

		location.x;
		location.y;

		break;

	case DeerState::LEFT:

		location.x -= 1;
		DeerAnim();
		break;

	case DeerState::RIGHT:

		location.x += 1;
		DeerAnim();
		break;

	case DeerState::DEATH:

		break;
	}
}

void EnemyDeer::Finalize()
{

}

//何かと当たった時の処理 _
//location=当たった相手の座標 _erea=当たった相手の大きさ _type=当たった相手の種類
// (0=ステージ(BLOCK) 1=プレイヤー(PLAYER) 2=敵 3=炎(FIRE) 4=木(WOOD) 5=水(WATER))
//_color_data=当たった相手も色情報
//自分の当たり判定を取っている部分は 一番下の辺
void EnemyDeer::Hit(Object* _object)
{
	/*DrawTest1 = _object->GetLocation().x;
	DrawTest2 = _object->GetLocation().y;
	DrawTest3 = _object->GetErea().y;
	DrawTest4 = _object->GetErea().x;
	DrawTest5 = _object->GetObjectType();
	DrawTest6 = _object->GetColerData();*/

	//ブロックと当たった時の処理
	if (
			(_object->GetObjectType() == BLOCK && _object->GetCanHit() == TRUE) ||
			(_object->GetObjectType() == FIRE && _object->GetCanSwap() == TRUE && this->color == RED) ||
			(_object->GetObjectType() == WOOD && _object->GetCanSwap() == TRUE && this->color == GREEN) ||
			(_object->GetObjectType() == WATER && _object->GetCanSwap() == TRUE && this->color == BLUE)
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
		erea.x = tmpe.x - 20.f;

		//プレイヤー上方向の判定
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][top]) {
			stageHitFlg[0][top] = true;
			stageHitFlg[1][top] = true;
		}
		else {
			stageHitFlg[0][top] = false;
		}

		//プレイヤー下方向の判定
		location.y += tmpe.y + 1;
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][bottom]) {
			stageHitFlg[0][bottom] = true;
			stageHitFlg[1][bottom] = true;

			if (!deer_spawn)
			{
				deer_state = DeerState::LEFT;
				deer_spawn = true;
			}
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


		location.y += move[top];
		location.y += move[bottom];


		//左右判定用に座標とエリアの調整
		//location.y += 20.f;
		erea.y = tmpe.y - 20.f;
		erea.x = 1;

		//プレイヤー左方向の判定
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][left]) {
			stageHitFlg[0][left] = true;
			stageHitFlg[1][left] = true;
			int a = CheckCollision(_object->GetLocation(), _object->GetErea());
			deer_state = DeerState::RIGHT;
		}
		else {
			stageHitFlg[0][left] = false;
		}


		//プレイヤー右方向の判定
		location.x = tmpl.x + tmpe.x + 1;
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][right]) {
			stageHitFlg[0][right] = true;
			stageHitFlg[1][right] = true;
			deer_state = DeerState::LEFT;
		}
		else {
			stageHitFlg[0][right] = false;
		}



		//最初の値に戻す

		location.x = tmpl.x;
		//location.y += -20.f;
		erea.y = tmpe.y;
		erea.x = tmpe.x;



		//左方向に埋まらないようにする
		if (stageHitFlg[0][left]) {//左方向に埋まっていたら
			float t = (_object->GetLocation().x + _object->GetErea().x) - location.x;
			if (t != 0) {
				move[left] = t;
			}
		}

		//右方向に埋まらないようにする
		if (stageHitFlg[0][right]) {//右方向に埋まっていたら
			float t = _object->GetLocation().x - (location.x + erea.x);
			if (t != 0) {
				move[right] = t;
			}
		}


		//上下左右の移動量から移動後も埋まってるか調べる
		//左右移動させてみてまだ埋まってたら戻す
		location.x += move[left];
		location.x += move[right];
		if (location.x + erea.x < _object->GetLocation().x || location.x > _object->GetLocation().x + _object->GetErea().x) {
			if (stageHitFlg[1][top] || stageHitFlg[1][bottom]) {
				location.x -= move[left];
				location.x -= move[right];
			}
		}

		erea.y = tmpe.y;
		erea.x = tmpe.x;
	}

	// 死亡判定
	if (

		(this->color == RED && _object->GetObjectType() == WATER) ||
		(this->color == BLUE && _object->GetObjectType() == WOOD) ||
		(this->color == GREEN && _object->GetObjectType() == FIRE)

		)
	{
		if (deer_state != DeerState::DEATH)
		{
			deer_state = DeerState::DEATH;
			ResourceManager::StartSound(damage_se[_object->GetObjectType() - 3]);
			can_swap = false;
		}
	}

	//ダメージゾーンを上書きする
	if ((this->color == GREEN && _object->GetObjectType() == WATER && _object->GetCanSwap() == FALSE) ||
		(this->color == BLUE && _object->GetObjectType() == FIRE && _object->GetCanSwap() == FALSE) ||
		(this->color == RED && _object->GetObjectType() == WOOD && _object->GetCanSwap() == FALSE))
	{
		_object->SetColorData(color);
	}
}

bool EnemyDeer::CheckCollision(Vector2D l, Vector2D e)
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

void EnemyDeer::DeerAnim()
{
	// 足の回転方向を制御するフラグを追加
	for (int i = 0; i < 4; i++) {
		// 回転方向に基づいて角度を更新
		leg_angle[i] += speed;

		// 角度の範囲を制限する
		if (leg_angle[i] >= 25.0f) {
			leg_angle[i] = 25.0f - 1;
			//回転方向を反転
			speed = -speed;
		}
		if (leg_angle[i] <= -5.0f) {
			leg_angle[i] = -5.0f + 1;
			//回転方向を反転
			speed = -speed;
		}
	}
}
