#include "EnemyFrog.h"
#include <math.h>
#include"../../Scene/GameMain.h"
#include"../../Utility/ResourceManager.h"

EnemyFrog::EnemyFrog():
	timer(0),
	frog_state(FrogState::LEFT_JUMP), 
	velocity{0,0},
	death_timer(0), 
	old_location{0,0},
	faint_timer(0),
	jump_cooldown_timer(0),
	jump_timer(0),
	face_angle(0.0f),
	effect_once(false),
	jump_se(0)
{
	object_type = ENEMY;
	can_swap = TRUE;
	can_hit = TRUE;
}

EnemyFrog::~EnemyFrog()
{

}

void EnemyFrog::Initialize(Vector2D _location, Vector2D _erea, int _color_data)
{
	camera = Camera::Get();
	location = _location;
	erea = _erea;
	color = _color_data;

	stageHitFlg[1][bottom] = true;

	frog_image[0] = ResourceManager::SetGraph("Resource/Images/sozai/red_frog.PNG");
	frog_image[1] = ResourceManager::SetGraph("Resource/Images/sozai/green_frog.PNG");
	frog_image[2] = ResourceManager::SetGraph("Resource/Images/sozai/blue_frog.PNG");

	jump_image[0] = ResourceManager::SetGraph("Resource/Images/sozai/red_frog_jump.PNG");
	jump_image[1] = ResourceManager::SetGraph("Resource/Images/sozai/green_frog_jump.PNG");
	jump_image[2] = ResourceManager::SetGraph("Resource/Images/sozai/blue_frog_jump.PNG");

	damage_image[0] = ResourceManager::SetGraph("Resource/Images/sozai/red_frog_damage.PNG");
	damage_image[1] = ResourceManager::SetGraph("Resource/Images/sozai/green_frog_damage.PNG");
	damage_image[2] = ResourceManager::SetGraph("Resource/Images/sozai/blue_frog_damage.PNG");

	jump_se = ResourceManager::SetSound("Resource/Sounds/Enemy/frog_jump.wav");
	damage_se[0] = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_damage_fire.wav");
	damage_se[1] = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_damage_grass.wav");
	damage_se[2] = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_damage_water.wav");
	faint_se = ResourceManager::SetSound("Resource/Sounds/Enemy/enemy_faint.wav");
	fall_se = ResourceManager::SetSound("Resource/Sounds/Player/player_fall.wav");
	hit_se = ResourceManager::SetSound("Resource/Sounds/System/hit.wav");
}

void EnemyFrog::Update(ObjectManager* _manager)
{
	__super::Update(_manager);

	DebugInformation::Add("frog_old_x", (int)old_state);
	DebugInformation::Add("frog_x", (int)frog_state);
	//if (velocity.x != 0 || velocity.y != 0)
	//{
	//	_manager->SpawnEffect(location, erea, 1, 20, color);
	//}
	if (stageHitFlg[1][bottom] != true) { //重力
		velocity.y += 1.f;
		if (velocity.y > 16.f) {
			velocity.y = 16.f;
		}
		effect_once = false;
	}
	else {
		//一回だけエフェクトを出す
		if (effect_once == false)
		{
			_manager->SpawnEffect(location, erea, LandingEffect, 15, color);
			effect_once = true;
		}
		velocity.y = 0.f;
	}
	//落下死処理
	if (location.y > camera->GetStageSize().y)
	{
		frog_state = FrogState::DEATH;
		ResourceManager::StartSound(fall_se);
	}
	UpdataState(_manager);
	//カエルの状態に応じて更新
	switch (frog_state)
	{
	case FrogState::RIGHT_IDOL:
		jump_timer = 0;
		if (++jump_cooldown_timer > JUMP_COOLDOWN)
		{
			velocity.x = ((float)GetRand(5) + 3);
			velocity.y = -((float)GetRand(6) + 17);
			//ジャンプ前の座標を保存
			old_location = location;
			ResourceManager::StartSound(jump_se);
			jump_cooldown_timer = 0;
		}
		//空中で僅かに加速(ブロックに引っかかる対策)
		if (velocity.x == 0 && stageHitFlg[1][bottom] == false)
		{
			velocity.x = 0.6f;
		}
		//加速度を０まで緩やかに加算、減算する
		if (velocity.x > 0)velocity.x--;
		if (velocity.x < 0)velocity.x++;
		if (velocity.y > 0)velocity.y--;
		if (velocity.y < 0)velocity.y++;
		break;
	case FrogState::LEFT_IDOL:
		jump_timer = 0;
		if (++jump_cooldown_timer > JUMP_COOLDOWN)
		{
			velocity.x = -((float)GetRand(5) + 3);
			velocity.y = -((float)GetRand(6) + 17);
			//ジャンプ前の座標を保存
			old_location = location;
			ResourceManager::StartSound(jump_se);
			jump_cooldown_timer = 0;
		}
		//空中で僅かに加速(ブロックに引っかかる対策)
		if (velocity.x == 0 && stageHitFlg[1][bottom] == false)
		{
			velocity.x = -0.6f;
		}
		//加速度を０まで緩やかに加算、減算する
		if (velocity.x > 0)velocity.x--;
		if (velocity.x < 0)velocity.x++;
		if (velocity.y > 0)velocity.y--;
		if (velocity.y < 0)velocity.y++;
		break;
	case FrogState::LEFT_JUMP:
	case FrogState::RIGHT_JUMP:
		jump_timer++;
		break;
	case FrogState::FAINT:
		if (++faint_timer > FAINT_TIME)
		{
			faint_timer = 0;
			SetFrogState(FrogState::RIGHT_IDOL);
		}
		//加速度を０まで緩やかに加算、減算する
		if (velocity.x > 0)velocity.x--;
		if (velocity.x < 0)velocity.x++;
		break;
	case FrogState::DEATH:
		//その場で停止
		velocity = 0;
		_manager->SpawnEffect(location, erea, DeathEffect, 15, color);
		if (++death_timer > 60)
		{
			if (this != nullptr) {
				_manager->DeleteObject(this);
			}
		}
		break;
	default:
		break;
	}

	Move(_manager);

	for (int i = 0; i < 4; i++) {
		stageHitFlg[0][i] = false;
		stageHitFlg[1][i] = false;
	}
}

void EnemyFrog::Draw()const
{
	__super::Draw();

	////残像描画
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, (255 - (death_timer * 4)) - 500);
	//JumpFrogDraw({ local_location.x - (velocity.x * 2),local_location.y - (velocity.y * 2) },face_angle);

	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, (255 - (death_timer * 4)) - 100);
	//JumpFrogDraw({ local_location.x - (velocity.x * 3),local_location.y - (velocity.y * 3) }, face_angle);



	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//DrawFormatStringF(local_location.x, local_location.y, 0x000000, "%d",frog_state);

	//死亡演出中なら透明に
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (death_timer * 4));
	//蛙描画
	switch (frog_state)
	{
	case FrogState::RIGHT_IDOL:
		//IdolFrogDraw(local_location, (int)frog_state);
		DrawTurnGraphF(local_location.x, local_location.y, ResourceManager::GetGraph(frog_image[GetColorNum(color)]), TRUE);
		break;
	case FrogState::LEFT_IDOL:
		//IdolFrogDraw(local_location, (int)frog_state);
		DrawGraphF(local_location.x, local_location.y, ResourceManager::GetGraph(frog_image[GetColorNum(color)]), TRUE);
		break;
	case FrogState::LEFT_JUMP:
		//JumpFrogDraw(local_location, face_angle);	
		
		//カエル跳躍描画
		DrawRotaGraphF(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), 1.0f, face_angle / 60.f -90.f/60.f, ResourceManager::GetGraph(jump_image[GetColorNum(color)]), TRUE, TRUE);

		break;
	case FrogState::RIGHT_JUMP:

		//カエル跳躍描画
		DrawRotaGraphF(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), 1.0f, face_angle / 60.f + 90.f / 60.f, ResourceManager::GetGraph(jump_image[GetColorNum(color)]), TRUE, FALSE);


		//JumpFrogDraw(local_location, face_angle);
		//DrawRotaGraphF(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), 1.0f, face_angle, TRUE, TRUE);
		break;
	case FrogState::FAINT:
		//3フレーム毎に描画する（点滅）
		if (frame % 3 == 0)
		{
			//JumpFrogDraw(local_location, frame * 5);
			//カエル跳躍描画
			DrawRotaGraphF(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), 1.0f, (frame*5/60.f), ResourceManager::GetGraph(jump_image[GetColorNum(color)]), TRUE);
		}
		break;
	case FrogState::DEATH:
		if (old_state == FrogState::LEFT_IDOL || old_state == FrogState::LEFT_JUMP)
		{
			DrawRotaGraphF(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), 1.0f, face_angle / 60.f - 90.f / 60.f, ResourceManager::GetGraph(damage_image[GetColorNum(color)]), TRUE, TRUE);
		}
		else 
		{
			DrawRotaGraphF(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), 1.0f, face_angle / 60.f + 90.f / 60.f, ResourceManager::GetGraph(damage_image[GetColorNum(color)]), TRUE, FALSE);
		}
		break;
	default:
		break;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void EnemyFrog::Finalize()
{

}

void EnemyFrog::Hit(Object* _object)
{

	//プレイヤーと当たった時の処理
	if (_object->object_type == PLAYER)
	{
		//プレイヤーとの属性相性で処理を変える
		switch (CheckCompatibility(this, _object))
		{
			//不利の場合
		case -1:
			//スタン状態か死亡状態でないなら実行
			if (frog_state != FrogState::FAINT && frog_state != FrogState::DEATH)
			{
				//スタン状態になる
				SetFrogState(FrogState::FAINT);
				//スタンSE再生
				ResourceManager::StartSound(faint_se);
			}
			//プレイヤーが左にいるなら右にノックバック
			if (this->location.x > _object->GetLocation().x)
			{
				velocity.x = 10;
			}
			//プレイヤーが右にいるなら左にノックバック
			else
			{
				velocity.x = -10;
			}
			//プレイヤーが上にいるなら下にノックバック
			if (this->location.y > _object->GetLocation().y)
			{
				velocity.y = 10;
			}
			//プレイヤーが下にいるなら上にノックバック
			else
			{
				velocity.y = -10;
			}
			break;
			//あいこの場合
		case 0:
			//プレイヤーが左にいるなら右にノックバック
			if (this->location.x > _object->GetLocation().x)
			{
				velocity.x = 10;
			}
			//プレイヤーが右にいるなら左にノックバック
			else
			{
				velocity.x = -10;
			}
			//プレイヤーが上にいるなら下にノックバック
			if (this->location.y > _object->GetLocation().y)
			{
				velocity.y = 0;
			}
			//プレイヤーが下にいるなら上にノックバック
			else
			{
				velocity.y = -10;
			}
			break;
			//有利の場合
		case 1:
			break;
			//それ以外
		default:
			break;
		}
		
	}

	//ブロックと当たった時の処理
	if ((_object->object_type == BLOCK || _object->object_type == GROUND_BLOCK) && _object->can_hit == TRUE)
	{
		Vector2D tmpl = location;
		Vector2D tmpe = erea;
		move[0] = 0;
		move[1] = 0;
		move[2] = 0;
		move[3] = 0;

		//上下判定用に座標とエリアの調整
		/*location.x += 10.f;*/
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
				velocity.y = !velocity.y;
				move[top] = t;
				//ぶつかるSE再生
				ResourceManager::StartSound(hit_se);
			}
		}

		//下方向に埋まらないようにする
		if (stageHitFlg[0][bottom]) {//下方向に埋まっていたら
			float t = _object->GetLocation().y - (location.y + erea.y);
			if (t != 0) {
				velocity.y = 0.f;
				move[bottom] = t;
			}
		}


		//左右判定用に座標とエリアの調整
		/*location.y += 3.f;*/
		erea.y = tmpe.y - 20.f;
		erea.x = 1;

		//プレイヤー左方向の判定
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][left]) {
			stageHitFlg[0][left] = true;
			stageHitFlg[1][left] = true;
			int a = CheckCollision(_object->GetLocation(), _object->GetErea());

	
		}
		else {
			stageHitFlg[0][left] = false;
		}


		//プレイヤー右方向の判定
		location.x = tmpl.x + tmpe.x + 1;
		if (CheckCollision(_object->GetLocation(), _object->GetErea()) && !stageHitFlg[1][right]) {
			stageHitFlg[0][right] = true;
			stageHitFlg[1][right] = true;
		}
		else {
			stageHitFlg[0][right] = false;
		}

		//最初の値に戻す

		location.x = tmpl.x;
		//location.y += -3.f;
		erea.y = tmpe.y;
		erea.x = tmpe.x;



		//左方向に埋まらないようにする
		if (stageHitFlg[0][left]) {//左方向に埋まっていたら
			float t = (_object->GetLocation().x + _object->GetErea().x) - location.x;
			if (t != 0) {
				velocity.x = !velocity.x;
				move[left] = t;
			}
		}

		//右方向に埋まらないようにする
		if (stageHitFlg[0][right]) {//右方向に埋まっていたら
			float t = _object->GetLocation().x - (location.x + erea.x);
			if (t != 0) {
				velocity.x= !velocity.x;
				move[right] = t;
			}
		}


		//上下左右の移動量から移動後も埋まってるか調べる
		location.x += move[left];
		location.x += move[right];
		if (location.x + erea.x < _object->GetLocation().x || location.x > _object->GetLocation().x + _object->GetErea().x) {
			if (stageHitFlg[1][top] || stageHitFlg[1][bottom]) {
				location.x -= move[left];
				location.x -= move[right];
			}
		}
		location.y += move[top];
		location.y += move[bottom];

		erea.y = tmpe.y;
		erea.x = tmpe.x;

	}

	//弱点色に触れた時の処理
	if (
		(_object->object_type == FIRE && this->color == GREEN)||
		(_object->object_type == WATER && this->color == RED)||
		(_object->object_type == WOOD && this->color == BLUE)
	   )
	{
		//死亡状態へ
		if (frog_state != FrogState::DEATH)
		{
			SetFrogState(FrogState::DEATH);
			ResourceManager::StartSound(damage_se[_object->object_type - 3]);
			can_swap = FALSE;
		}
	}

	//ボス攻撃なら上書きしない為に処理終了
	if (_object->is_boss_attack)return;

	//ダメージゾーンを上書きする
	if ((this->color == GREEN && _object->object_type == WATER && _object->can_swap == FALSE) ||
		(this->color == BLUE && _object->object_type == FIRE && _object->can_swap == FALSE) ||
		(this->color == RED && _object->object_type == WOOD && _object->can_swap == FALSE)
		)
	{
		_object->SetColorData(color);
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
			//相手が左にいるなら右にノックバック
			if (this->location.x > _object->GetLocation().x)
			{
				velocity.x = 10;
			}
			//相手が右にいるなら左にノックバック
			else
			{
				velocity.x = -10;
			}
			//相手が上にいるなら下にノックバック
			if (this->location.y > _object->GetLocation().y)
			{
				velocity.y = 10;
			}
			//相手が下にいるなら上にノックバック
			else
			{
				velocity.y = -10;
			}
			//ぶつかるSE再生
			ResourceManager::StartSound(hit_se);
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

void EnemyFrog::JumpFrogDraw(Vector2D location, float _angle)const
{
#ifdef _DEBUG
	//当たり判定
	//DrawBoxAA(location.x, location.y, location.x + erea.x, location.y + erea.y, color, FALSE);
	//移動方向
	//DrawFormatStringF(local_location.x, local_location.y, 0x00ff00, "%f %f", vector.x, vector.y);
#endif

		//目
	int eye_color;
	//瞬き
	eye_color = (frame / 2) % 10 == 0 ? color : 0xffffff;

	//足の位置
	int leg_pos;
	leg_pos = jump_timer < 10 ? jump_timer : 10;

	//胴体
	if (jump_cooldown_timer < JUMP_COOLDOWN/2)
	{
		ResourceManager::DrawRotaBox(location.x + (erea.x / 2), location.y + (erea.y / 2), erea.x, erea.y / 2, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + (erea.x / 2), location.y + (erea.y / 2), erea.x, erea.y / 2, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, 0x000000, FALSE);
	}
	else
	{
		ResourceManager::DrawRotaBox(location.x + (erea.x / 2)+GetRand(2), location.y + (erea.y / 2) + GetRand(2), erea.x, erea.y / 2, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + (erea.x / 2) + GetRand(2), location.y + (erea.y / 2) + GetRand(2), erea.x, erea.y / 2, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, 0x000000, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,jump_cooldown_timer*4);
		DrawCircleAA(location.x + (erea.x / 2), location.y + (erea.y / 2), (float)((JUMP_COOLDOWN - jump_cooldown_timer) * 4), 30.0f, color, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	//左向き
	if (face_angle > 90 && face_angle < 270)
	{
		//付け根側後ろ足
		ResourceManager::DrawRotaBox(location.x + erea.x + leg_pos, location.y + erea.y - 20, 30, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.x + leg_pos, location.y + erea.y - 20, 30, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, 0x000000, FALSE);
		//後ろ足先端
		ResourceManager::DrawRotaBox(location.x + erea.x + leg_pos*4.5f, location.y + erea.y - 10, 40, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.x + leg_pos*4.5f, location.y + erea.y - 10, 40, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, 0x000000, FALSE);
		//前足
		ResourceManager::DrawRotaBox(location.x, location.y + erea.y - 10, 10, 15, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, color, TRUE);
		ResourceManager::DrawRotaBox(location.x, location.y + erea.y - 10, 10, 15, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, 0x000000, FALSE);
		//目
		ResourceManager::DrawRotaBox(location.x, location.y + 20, 10, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, eye_color, TRUE);

	}
	//右向き
	else
	{
		//付け根側後ろ足
		ResourceManager::DrawRotaBox(location.x + erea.x + leg_pos, location.y + 20, 30, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.x + leg_pos, location.y + 20, 30, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, 0x000000, FALSE);
		//後ろ足先端
		ResourceManager::DrawRotaBox(location.x + erea.x + leg_pos * 4.5f, location.y + 10, 40, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.x + leg_pos * 4.5f, location.y + 10, 40, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, 0x000000, FALSE);
		//前足
		ResourceManager::DrawRotaBox(location.x, location.y + 10, 10, 15,location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, color, TRUE);
		ResourceManager::DrawRotaBox(location.x, location.y + 10, 10, 15,location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, 0x000000, FALSE);
		//目
		ResourceManager::DrawRotaBox(location.x, location.y + erea.y - 20, 10, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), _angle, eye_color, TRUE);

	}
}

void EnemyFrog::IdolFrogDraw(Vector2D location, bool _direction)const
{
	//胴体
	if (jump_cooldown_timer < JUMP_COOLDOWN / 2)
	{
		ResourceManager::DrawRotaBox(location.x + (erea.x / 2), location.y + (erea.y / 2), erea.x, erea.y / 2, location.x + (erea.x / 2), location.y + (erea.y / 2), 0, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + (erea.x / 2), location.y + (erea.y / 2), erea.x, erea.y / 2, location.x + (erea.x / 2), location.y + (erea.y / 2), 0, 0x000000, FALSE);
	}
	else
	{
		ResourceManager::DrawRotaBox(location.x + (erea.x / 2) + GetRand(2), location.y + (erea.y / 2) + GetRand(2), erea.x, erea.y / 2, location.x + (erea.x / 2), location.y + (erea.y / 2), 0, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + (erea.x / 2) + GetRand(2), location.y + (erea.y / 2) + GetRand(2), erea.x, erea.y / 2, location.x + (erea.x / 2), location.y + (erea.y / 2), 0, 0x000000, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, jump_cooldown_timer * 4);
		DrawCircleAA(location.x + (erea.x / 2), location.y + (erea.y / 2), (JUMP_COOLDOWN - jump_cooldown_timer) * 4, 30, color, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	//目
	int eye_color;
	//瞬き
	eye_color = (frame / 2) % 10 == 0 ? color : 0xffffff;


	//左着地
	if(_direction)
	{
		//付け根側後ろ足
		ResourceManager::DrawRotaBox(location.x + erea.x - 10, location.y + erea.y - 20, 30, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), 180, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.x - 10, location.y + erea.y - 20, 30, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), 180, 0x000000, FALSE);
		//後ろ足先端
		ResourceManager::DrawRotaBox(location.x + erea.x - 10, location.y + erea.y - 10, 40, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), 180, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.x - 10, location.y + erea.y - 10, 40, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), 180, 0x000000, FALSE);
		//前足
		ResourceManager::DrawRotaBox(location.x + 10, location.y + erea.y - 10, 10, 15, location.x + (erea.x / 2), location.y + (erea.y / 2), 180, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + 10, location.y + erea.y - 10, 10, 15, location.x + (erea.x / 2), location.y + (erea.y / 2), 180, 0x000000, FALSE);
		//目
		ResourceManager::DrawRotaBox(location.x, location.y + 20, 10, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), 180, eye_color, TRUE);

	}
	//右着地
	else
	{
		//付け根側後ろ足
		ResourceManager::DrawRotaBox(location.x + erea.x - 10, location.y + 20, 30, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), 0, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.x - 10, location.y + 20, 30, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), 0, 0x000000, FALSE);
		//後ろ足先端
		ResourceManager::DrawRotaBox(location.x + erea.x - 10, location.y + 10, 40, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), 0, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + erea.x - 10, location.y + 10, 40, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), 0, 0x000000, FALSE);
		//前足
		ResourceManager::DrawRotaBox(location.x + 10, location.y + 10, 10, 15, location.x + (erea.x / 2), location.y + (erea.y / 2), 0, color, TRUE);
		ResourceManager::DrawRotaBox(location.x + 10, location.y + 10, 10, 15, location.x + (erea.x / 2), location.y + (erea.y / 2), 0, 0x000000, FALSE);

		ResourceManager::DrawRotaBox(location.x, location.y + erea.y - 20, 10, 10, location.x + (erea.x / 2), location.y + (erea.y / 2), 0, eye_color, TRUE);
	}
}

void EnemyFrog::Move(ObjectManager* _manager)
{
	//顔の向き更新
	if (velocity.x == 0 && velocity.y == 0)
	{
		if (frog_state == FrogState::LEFT_JUMP)
		{
			face_angle = 180;
		}
		if (frog_state == FrogState::RIGHT_JUMP)
		{
			face_angle = 0;
		}
	}
	else
	{
		if (frog_state == FrogState::LEFT_JUMP)
		{
			face_angle = atanf(velocity.y / velocity.x) * 60;
		}
		if (frog_state == FrogState::RIGHT_JUMP)
		{

			face_angle = atanf(velocity.y / velocity.x) * 60;
		}
	}
	//移動
	location.x += velocity.x;
	location.y += velocity.y;
}

void EnemyFrog::UpdataState(ObjectManager* _manager)
{
	//死んでいたら更新を止める
	if (frog_state == FrogState::DEATH)return;

	//スタン中以外はステートの更新をする
	if (frog_state != FrogState::FAINT)
	{
		//地面についているか判断
		if (stageHitFlg[1][bottom])
		{
			//カエルとプレイヤーの座標を比較して、カエルがプレイヤーに向かって行くように
			if (location.x > _manager->GetPlayerLocation().x)
			{
				//ジャンプ後にX座標が変わらなければ、反対方向に飛ぶ
				if (old_location.x == location.x)
				{
					if (frog_state == FrogState::RIGHT_JUMP)SetFrogState(FrogState::LEFT_IDOL);
					else if (frog_state == FrogState::LEFT_JUMP)SetFrogState(FrogState::RIGHT_IDOL);
				}
				else
				{
					SetFrogState(FrogState::LEFT_IDOL);
				}
			}
			else
			{
				//ジャンプ後にX座標が変わらなければ、反対方向に飛ぶ
				if (old_location.x == location.x)
				{
					if (frog_state == FrogState::RIGHT_JUMP)SetFrogState(FrogState::LEFT_IDOL);
					else if (frog_state == FrogState::LEFT_JUMP)SetFrogState(FrogState::RIGHT_IDOL);
				}
				else
				{
					SetFrogState(FrogState::RIGHT_IDOL);
				}
			}
		}
		//地面についていなくて、StateがIDOLなら変更する
		else
		{
			if (frog_state == FrogState::LEFT_IDOL)
			{
				SetFrogState(FrogState::LEFT_JUMP);
			}
			if (frog_state == FrogState::RIGHT_IDOL)
			{
				SetFrogState(FrogState::RIGHT_JUMP);
			}
		}
	}

}

bool EnemyFrog::CheckCollision(Vector2D l, Vector2D e)
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

void EnemyFrog::SetFrogState(FrogState _state)
{
	//同じ状態以外に変更するなら、ひとつ前の状態を格納
	if (_state != frog_state)old_state = frog_state;
	frog_state = _state;
}