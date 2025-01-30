#include "Player.h"
#include "../Utility/UserData.h"
#include "../Utility/PadInput.h"
#include "../Utility/KeyInput.h"
#include "../Utility/ResourceManager.h"
#include "../Scene/GameMain.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../Utility/DebugInfomation.h"

Player::Player()
{
	camera = Camera::Get();
	frame = 0;
	object_type = PLAYER;
	can_swap = TRUE;	//プレイヤーのcan_swapは真でも偽でも大丈夫
	can_hit = TRUE;

	velocity = { 0,0 };
	saveVec = velocity;

	for (int i = 0; i < 4; i++) {
		stageHitFlg[1][i] = false;
		oldStick[i] = 0.f;
	}

	searchedObj = nullptr;
	searchFlg = false;
	swapTimer = -1;
	oldSearchFlg = false;

	for (int i = 0; i < OBJECT_NUM; i++)
	{
		searchedObjAll[i] = nullptr;
	}
	objNum = 0;
	for (int i = 0; i < 19; i++){
		for (int j = 0; j < 32; j++){
			posRelation[i][j] = -1;
		}
	}

	objSelectNumTmp = 0;
	searchedObjFlg = false;
	effect_once = false;
	swap_once = false;
	damageFlg = false;
	damageOldFlg = false;
	hp = 5;
	damageEffectTime = 90.f;
	damageEffectFlg = false;
	state = 0;
	stateFlg = false;
	fps = 0;
	for (int i = 0; i < 4; i++)
	{
		angle[i] = 0.f;
	}
	p_state = PlayerState::MOVE_RIGHT;
	pStateOld = PlayerState::MOVE_RIGHT;
	moveFrontFlg = true;
	animFlg = false;	
	circleAng = 0.f;

	spawn_anim_timer = 0;
	warp_anim_timer = 0;

	matchup_image[0] = ResourceManager::SetGraph("Resource/Images/sozai/matchup_red.PNG");
	matchup_image[1] = ResourceManager::SetGraph("Resource/Images/sozai/matchup_green.PNG");
	matchup_image[2] = ResourceManager::SetGraph("Resource/Images/sozai/matchup_blue.PNG");

	spawn_se = ResourceManager::SetSound("Resource/Sounds/Player/player_warp.wav");
	warphole_se = ResourceManager::SetSound("Resource/Sounds/Effect/warphole_spawn.wav");
	landing_se = ResourceManager::SetSound("Resource/Sounds/Player/walk_normal.wav");
	walk_se[0] = ResourceManager::SetSound("Resource/Sounds/Player/walk_normal.wav");
	walk_se[1] = ResourceManager::SetSound("Resource/Sounds/Player/walk_fire.wav");
	walk_se[2] = ResourceManager::SetSound("Resource/Sounds/Player/walk_grass.wav");
	walk_se[3] = ResourceManager::SetSound("Resource/Sounds/Player/walk_water.wav");
	jump_se = ResourceManager::SetSound("Resource/Sounds/Player/player_jump.wav");
	base_damage_se = ResourceManager::SetSound("Resource/Sounds/Player/player_damage.wav");
	damage_se[0] = ResourceManager::SetSound("Resource/Sounds/Player/damage_water.wav");
	damage_se[1] = ResourceManager::SetSound("Resource/Sounds/Player/damage_fire.wav");
	damage_se[2] = ResourceManager::SetSound("Resource/Sounds/Player/damage_grass.wav");
	cursor_se = ResourceManager::SetSound("Resource/Sounds/Player/cursor.wav");
	fall_se = ResourceManager::SetSound("Resource/Sounds/Player/player_fall.wav");
	hit_se = ResourceManager::SetSound("Resource/Sounds/System/hit.wav");
	hit_se = ResourceManager::SetSound("Resource/Sounds/System/hit.wav");

	//プレイヤーの画像の読み込み
	LoadPlayerImage();

	now_riding = 0;
	color = 0;

	emoteFlg = false;
	emoteCnt = 0;

	//スポーン演出開始
	spawn_anim_flg = true;
}

Player::~Player()
{
	
}

void Player::Initialize(Vector2D _location, Vector2D _erea, int _color_data)
{
	location = _location;
	erea = _erea;
	color = _color_data;

	damageFlg = false;
}

void Player::Update(ObjectManager* _manager)
{
	//ステージ遷移時のリセット処理
	if (_manager->player_respawn_flg)
	{
		PlayerReset(_manager);
	}

	//スポーン演出処理
	if (spawn_anim_flg)
	{
		//各SEの再生
		if (spawn_anim_timer == 2)
		{
			//スポーン演出の効果音再生
			ResourceManager::StartSound(warphole_se);
		}
		if (spawn_anim_timer == PLAYER_SPAWN_ANIM)
		{
			//スポーン演出の効果音再生
			ResourceManager::StartSound(spawn_se);
		}
		//演出が終了したら
		if (++spawn_anim_timer > SPAWN_ANIM_TIME || PadInput::OnButton(XINPUT_BUTTON_B))
		{
			//フラグを下げてタイマーをリセット
			spawn_anim_flg = false;
			spawn_anim_timer = 0;
		}
		//スポーン演出中はプレイヤーのアップデートはしない
		return;
	}
	//遷移演出処理
	if (_manager->player_warp_flg)
	{
		//SE再生
		if (warp_anim_timer == 0)
		{
			//スポーン演出の効果音再生
			ResourceManager::StartSound(warphole_se);
		}
		//演出が終了したら
		if (++warp_anim_timer > WARP_ANIM_TIME)
		{
			//フラグを下げて、タイマーをリセット
			_manager->player_warp_flg = false;
			warp_anim_timer = 0;
		}
		//遷移演出中はプレイヤーのアップデートはしない
		return;
	}

	if (!is_tutorial) {
		__super::Update(_manager);

		if (searchFlg) {
			if (fps++ > 6000) {
				fps = 0;
			}
		}
		else {
			fps = 0;
		}

		//移動エフェクト
		if (velocity.x != 0 || velocity.y != 0)
		{
			_manager->SpawnEffect(location, erea, ShineEffect, 20, color);
		}

		if (stageHitFlg[1][bottom] != true) { //重力
			switch (state)
			{
			case 0:
				if (searchFlg) {
					velocity.y += 1.f * 0.02f;
				}
				else {
					velocity.y += 1.f;
				}
				if (velocity.y > 16.f) {
					velocity.y = 16.f;
				}
				break;

			case 1:
				if (searchFlg) {
					velocity.y += 0.5f * 0.02f;
				}
				else {
					velocity.y += 0.5f;
				}
				if (velocity.y > 8.f) {
					velocity.y = 8.f;
				}
				break;

			default:
				break;
			}

			effect_once = false;
		}
		else {
			//一回だけエフェクトを出す
			if (effect_once == false)
			{
				_manager->SpawnEffect(location, erea, LandingEffect, 15, color);
				ResourceManager::StartSound(landing_se);
				effect_once = true;
			}
			velocity.y = 0.f;
		}

		if (objNum <= 0) {
			searchedObjFlg = false;
		}
		else {
			searchedObjFlg = true;
		}

		oldSearchFlg = searchFlg;
		//Bボタンで色の交換ができるモードと切り替え
		if (UserData::CheckActionKey((int)PlayerActionKey::P_SWAP,PRESSED) && /*!_manager->GetPauseAfter() &&*/ swapTimer < 0) {
			SelectObject();
			searchFlg = true;
		}
		else if (UserData::CheckActionKey((int)PlayerActionKey::P_SWAP, RELEASE) && searchFlg && searchedObj != nullptr && swapTimer < 0) {
			//交換エフェクトにかかる時間を受け取る
			if (searchedObj != this)
			{
				swapTimer = _manager->Swap(this, searchedObj);
				objSelectNumTmp = 0;

				//描画する色を白に
				//color = WHITE;
			}
			else
			{
				swapTimer = 0;
			}
		}
		else if (UserData::CheckActionKey((int)PlayerActionKey::P_SWAP, RELEASE) && swapTimer < 0) {//交換できるオブジェクトが画面内になかった時
			searchFlg = false;
		}

		//交換後エフェクト用の硬直
		if (swapTimer >= 0)
		{
			if (swapTimer <= SWAP_EFFECT_STOP_TIME)
			{
				if (swap_once == false)
				{
					ChangePlayerColor();
					swap_once = true;
					emoteFlg = true;
				}
			}

			//硬直が終わったら色を交換
			if (--swapTimer < 0)
			{
				searchFlg = false;
				swapTimer = -1;
				searchedObj = nullptr;
				swap_once = false;
				//emoteFlg = false;
			}

		}

		//交換ポーズカウント
		if (emoteFlg) {
			emoteCnt++;
			if (emoteCnt >= 15) {
				emoteCnt = 0;
				emoteFlg = false;
			}
		}

		pStateOld = p_state;
		if (hp > 0) {
			MoveActor();
		}

		PlayerAnim();
		AnimStateUpdate();	//アニメーションの状態更新

		//加速度が早すぎる場合移動しない
		if (fabsf(velocity.x) > 50)velocity.x = 0;
		if (fabsf(velocity.y) > 50)velocity.y = 0;

		if (searchFlg) {
			location.x += velocity.x * 0.02f;
			location.y += velocity.y * 0.02f;
		}
		else {
			location.x += velocity.x;
			location.y += velocity.y;
		}

		//交換中だけ対象のオブジェクトの見た目を白くする
		//if (swapTimer > SWAP_EFFECT_STOP_TIME)
		//{
		//	//選択中のオブジェクトを更新
		//	_manager->SetNowCurrentObject(searchedObj);
		//}
		//else
		//{
		//	_manager->SetNowCurrentObject(nullptr);
		//}
		//音声の周波数設定
		if (searchFlg == TRUE)
		{
			ResourceManager::SetSoundFreq(8000);
		}
		else
		{
			ResourceManager::SetSoundFreq(DEFAULT_FREQ);
		}

		PlayerSound();		//音声再生関連処理


		//damage
		if (damageFlg && !damageOldFlg/* && d == 1*/) {
			if (damageEffectFlg == false) {
				
				damageEffectFlg = true;
				hp--;
			}
		}
		if (damageEffectFlg) {
			if ((int)damageEffectTime == 90) {

				camera->SetImpact(30);
				//HPが０なら、エフェクトをスポーンさせない
				if (hp != 0)
				{
					_manager->SpawnEffect(location, erea, DamageEffect, 20, color);
					_manager->SpawnEffect(location, erea, DamageEffect, 20, color);
					_manager->SpawnEffect(location, erea, DamageEffect, 20, color);
				}
			}
			//交換中はダメージエフェクトもゆっくり
			if (searchFlg)damageEffectTime -= 0.1f;
			else damageEffectTime -= 1.f;

			if (damageEffectTime <= 0) {
				damageEffectFlg = false;
				damageEffectTime = 90.f;
				damageFlg = false;
			}
		}

		for (int i = 0; i < 4; i++) {
			stageHitFlg[0][i] = false;
			stageHitFlg[1][i] = false;
		}

		objNum = 0;
		for (int i = 0; i < 19; i++) {
			for (int j = 0; j < 32; j++) {
				posRelation[i][j] = -1;
			}
		}

		damageOldFlg = damageFlg;
		damageFlg = false;

		stateFlg = false;
		state = 0;
		now_riding = 0;

		if (circleAng++ >= 360.f) {
			circleAng = 0.f;
		}
		//HP0か落下でゲームオーバー
		if (hp <= 0 || location.y > camera->GetStageSize().y +100) {
			
			searchFlg = false;
			velocity.x = 0.f;
			velocity.y = 0.f;
			if (deathTimer++ == 0)
			{
				ResourceManager::StartSound(fall_se);
				_manager->UpdateState(GameMainState::GameOver);
			}
		}
	}

	//ボス演出中ならプレイヤーを停止させる
	if (_manager->boss_appeared_flg)p_state = PlayerState::IDLE_RIGHT;
}

void Player::Draw()const
{

	for (int i = 0; i < hp; i++)
	{
		ResourceManager::DrawHeart({ 50.f + i * 50.f,75.f }, { 40.f,40.f });
	}
	//登場演出の描画
	if (spawn_anim_flg)
	{
		int oval_size = (spawn_anim_timer * 2);
		if (oval_size > 40)oval_size = 40;
		//DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, GetColor(0, 0, GetRand(255)), true);
		DrawOvalAA(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), oval_size * 0.7f,      oval_size * 1.5f, 30, 0x000066, true);
		DrawOvalAA(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), oval_size * 0.7f - 5,  oval_size * 1.5f - 5, 30, 0x000055, true);
		DrawOvalAA(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), oval_size * 0.7f - 10, oval_size * 1.5f - 10, 30, 0x000044, true);
		DrawOvalAA(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), oval_size * 0.7f - 15, oval_size * 1.5f - 15, 30, 0x000033, true);
		DrawOvalAA(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), oval_size * 0.7f - 20, oval_size * 1.5f - 20, 30, 0x000022, true);
		DrawOvalAA(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), oval_size * 0.7f - 25, oval_size * 1.5f - 25, 30, 0x000011, true);
		DrawOvalAA(local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), oval_size * 0.7f - 30, oval_size * 1.5f - 30, 30, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, spawn_anim_timer*2.5f- PLAYER_SPAWN_ANIM);
	}
	//遷移演出中の描画
	if (warp_anim_timer > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - warp_anim_timer * 2.5f);
	}
	__super::Draw();

	/*SetFontSize(35);
	DrawFormatString(0, 220, 0xff0000, "%0.1f %0.1f", location.x, location.y);*/

	if(hp <= 0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (deathTimer * 2));
	}
	
	//プレイヤー描画
	DrawPlayerImage();

	//DrawPlayerFront(true);

	//色交換カーソル
	if (searchedObj != nullptr && searchFlg) {
		DrawCircleAA(searchedObj->GetLocalLocation().x + searchedObj->GetErea().x / 2,
			searchedObj->GetLocalLocation().y + searchedObj->GetErea().y / 2, 40, 40, 0xffff00, FALSE, 4);

		Vector2D base;
		base.x = searchedObj->GetLocalLocation().x + searchedObj->GetErea().x / 2;
		base.y = searchedObj->GetLocalLocation().y + searchedObj->GetErea().y / 2;

		Vector2D l[3];
		l[0].x = searchedObj->GetLocalLocation().x + searchedObj->GetErea().x / 2;
		l[0].y = (searchedObj->GetLocalLocation().y + searchedObj->GetErea().y / 2) - 40;

		l[0] = RotationLocation(base, l[0], (float)(circleAng * M_PI / 180));

		l[1] = RotationLocation(base, l[0], (float)(120.f * M_PI / 180));

		l[2] = RotationLocation(base, l[0], (float)(240.f * M_PI / 180));
		
		
		DrawCircleAA(l[0].x, l[0].y, 15, 32, 0xcc0000, TRUE);
		DrawCircleAA(l[1].x, l[1].y, 15, 32, 0x3c78d8, TRUE);
		DrawCircleAA(l[2].x, l[2].y, 15, 32, 0x6aa84f, TRUE);
	
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	if (searchFlg) {
		int j = 0;
		for (int i = fps; i > 0; i--)
		{
			if (j++ > 10) {
				j = 10;
				break;
			}
		}

		int _erea = 20;
		for (int i = 0; i < j; i++)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 110 - i * 10);
			DrawBox(i * _erea, i * _erea, SCREEN_WIDTH - i * _erea, i * _erea + _erea, 0x000000, true);//上
			DrawBox(i * _erea, SCREEN_HEIGHT - i * _erea - _erea, SCREEN_WIDTH - i * _erea, SCREEN_HEIGHT - i * _erea, 0x000000, true);//下
			DrawBox(i * _erea, i * _erea + _erea, i * _erea + _erea, SCREEN_HEIGHT - i * _erea - _erea, 0x000000, true);//左
			DrawBox(SCREEN_WIDTH - i * _erea - _erea, i * _erea + _erea, SCREEN_WIDTH - i * _erea, SCREEN_HEIGHT - i * _erea - _erea, 0x000000, true);//右
		}
	}
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//色に合わせて相性表の描画
	DrawGraph(SCREEN_WIDTH - 320, 0, ResourceManager::GetGraph(matchup_image[GetColorNum(color)]), true);
}

void Player::Finalize()
{
}

void Player::Hit(Object* _object)
{
	//敵と当たった時の処理
	if (_object->object_type == ENEMY)
	{
		//プレイヤーとの属性相性で処理を変える
		switch (CheckCompatibility(this, _object))
		{
			//不利の場合
		case -1:
			//ダメージ
			if (!damageEffectFlg &&
				(_object->can_hit || _object->is_boss_attack == TRUE)) {

				damageFlg = true;
				//ノックバック
				//プレイヤーが右にいるなら右にノックバック
				if (this->location.x > _object->GetLocation().x)
				{
					velocity.x += 10;
				}
				//プレイヤーが左にいるなら左にノックバック
				else
				{
					velocity.x -= 10;
				}
				velocity.y -= 10;
			}
			break;
			//あいこの場合
		case 0:
			//プレイヤーが右にいるなら右にノックバック
			if (this->location.x > _object->GetLocation().x)
			{
				velocity.x = 10;
			}
			//プレイヤーが左にいるなら左にノックバック
			else
			{
				velocity.x = -10;
			}
			//ぶつかったSE再生
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

	//ブロックと当たった時の処理
	if (
			(
				(_object->object_type == BLOCK || _object->object_type == GROUND_BLOCK) && _object->can_hit == TRUE)||
				(_object->can_swap == TRUE && _object->CheckCompatibility(this,_object) == 0
			)
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
				velocity.y = 0.f;
				move[top] = t;
			}
		}

		//下方向に埋まらないようにする
		if (stageHitFlg[0][bottom]) {//下方向に埋まっていたら
			int t = (int)_object->GetLocation().y - (location.y + erea.y);
			if (t != 0) {
				move[bottom] = (float)t;
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
		//location.y += -20.f;
		erea.y = tmpe.y;
		erea.x = tmpe.x;



		//左方向に埋まらないようにする
		if (stageHitFlg[0][left]) {//左方向に埋まっていたら
			float t = (_object->GetLocation().x + _object->GetErea().x) - location.x;
			if (t != 0) {
				velocity.x = 0.f;
				move[left] = t;
			}
		}

		//右方向に埋まらないようにする
		if (stageHitFlg[0][right]) {//右方向に埋まっていたら
			float t = _object->GetLocation().x - (location.x + erea.x);
			if (t != 0) {
				velocity.x = 0.f;
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

	//不利な属性のブロックかダメージゾーンと当たった時の処理
	if ((_object->object_type == FIRE || _object->object_type == WOOD || _object->object_type == WATER) && CheckCompatibility(this, _object) == -1)
	{
		//演出中ならダメージを受けない
		if (!damageEffectFlg)
		{
			damageFlg = true;
		}
		//触れた色に応じてSEを変える
		now_riding = GetColorNum(_object->GetColorData());
	}

	//ダメージゾーンを上書きする
	if ((_object->object_type == WATER && _object->can_swap == FALSE && this->color == GREEN) ||
		(_object->object_type == FIRE && _object->can_swap == FALSE && this->color == BLUE) ||
		(_object->object_type == WOOD && _object->can_swap == FALSE && this->color == RED))
	{
		if (!_object->is_boss_attack)
		{
			_object->SetColorData(color);
		}
	}

	//同じ属性のダメージゾーン内で繰り返しジャンプ出来る
	if (((_object->object_type == WATER && _object->can_swap == FALSE && this->color == BLUE && !stateFlg) ||
		(_object->object_type == FIRE && _object->can_swap == FALSE && this->color == RED && !stateFlg) ||
		(_object->object_type == WOOD && _object->can_swap == FALSE && this->color == GREEN && !stateFlg)) && !_object->is_boss_attack){
		state = 1;
	}

	//自分が乗っている(触れている)ブロックに応じてSEを変える
	if ((_object->object_type == FIRE && this->color != RED) || _object->object_type == WOOD || _object->object_type == WATER)
	{
		now_riding = _object->object_type - 2;
	}
	else if(_object->object_type == FIRE && this->color == RED)
	{
		now_riding = 0;
	}

	//ボスの木攻撃に当たった時、プレイヤーを跳ねさせる
	if (_object->object_type == BLOCK && _object->is_boss_attack == TRUE && _object->GetLocation().y > this->location.y)
	{
		velocity.y = -20;
	}

	//チュートリアル
	if (_object->object_type == TUTORIAL)
	{
		//is_tutorial = true;
	}
}

void Player::MoveActor()
{
	//ジャンプ
	if (UserData::CheckActionKey((int)PlayerActionKey::P_JUMP, ON_BUTTON) && (damageEffectTime == 90.f || damageEffectTime <45.f)) {
		switch (state)
		{
		case 0:
			if (stageHitFlg[1][bottom]) {
				velocity.y = -23.f;
			}
			break;

		case 1:
			velocity.y = -23.f;

			break;

		default:
			break;
		}
	
	}

	//左右移動
	float stick;
	if (!searchFlg) {
		//サーチ状態の時のベクトルを代入
		if (oldSearchFlg) {
			velocity = saveVec;
		}

		if (UserData::CheckActionKey((int)PlayerActionKey::P_WALK_RIGHT, PRESSED) && (damageEffectTime == 90.f || damageEffectTime < 45.f)) {
			//stick = PadInput::TipLStick(STICKL_X);
			velocity.x += 1.f;	//スティックの角度をかけていたのを削除
			if (velocity.x > 6.5f) {
				velocity.x = 6.5f;
			}
			moveFrontFlg = true;
		}
		else if (UserData::CheckActionKey((int)PlayerActionKey::P_WALK_LEFT, PRESSED) && (damageEffectTime == 90.f || damageEffectTime < 45.f)) {
			//stick = PadInput::TipLStick(STICKL_X);
			velocity.x -= 1.f;//スティックの角度をかけていたのを削除
			if (velocity.x < -6.5f) {
				velocity.x = -6.5f;
			}
			moveFrontFlg = false;
		}
		else {
			if (velocity.x > 0.1f) {
				velocity.x += -0.3f;
			}
			else if (velocity.x < 0.1f) {
				velocity.x += 0.3f;
			}


			if (velocity.x > -0.4f && velocity.x < 0.4f) {
				velocity.x = 0.f;
			}
		}
	}
	else {
		if (!oldSearchFlg) {
			saveVec = velocity;
		}
	}

	//if (stageHitFlg[1][bottom] != true) {
	//	pState = jump;
	//}
}

bool Player::SearchColor(Object* ob)
{
	if (ob != nullptr && ob->GetColorData() > 0 /*&& ob != this*/){
		// 最初にプレイヤーを追加
		if (objNum == 0) 
		{  
			searchedObjAll[objNum] = this;
			int x = (int)this->GetLocalLocation().x / 40;
			int y = (int)this->GetLocalLocation().y / 40;
			posRelation[y][x] = objNum;
			objNum++;
		}
		//ボスも交換対象に追加
		if (ob->object_type == BOSS)
		{
			searchedObjAll[objNum] = ob;
			int x = (int)ob->GetLocalLocation().x / 40;
			int y = (int)ob->GetLocalLocation().y / 40;
			posRelation[y][x] = objNum;
			objNum++;
		}
		else if(ob->GetLocalLocation().x >= 0 && ob->GetLocalLocation().x <= 1280 && ob->GetLocalLocation().y >= 0 && ob->GetLocalLocation().y <= 720) 
		{
			searchedObjAll[objNum] = ob;
			int x = (int)ob->GetLocalLocation().x / 40;
			int y = (int)ob->GetLocalLocation().y / 40;
			posRelation[y][x] = objNum;
			objNum++;
		}
	}

	// プレイヤーを最初に選択対象として追加
	return false;
}

bool Player::ChangePlayerColor()
{
	if (this != nullptr && searchedObj != nullptr) {
		ChangeColor(searchedObj);
		if (color == RED)
		{
			SetWindowIconID(101);
		}
		else if (color == GREEN)
		{
			SetWindowIconID(102);
		}
		else if (color == BLUE)
		{
			SetWindowIconID(103);
		}
		return true;
	}
	return false;	
}

void Player::SelectObject()
{
	bool flg = false;//選択したかどうか
	
	// スワップタイマーが未設定で、オブジェクトが検索されている場合の処理
	if (swapTimer == -1 && searchedObjFlg && searchedObj != nullptr) {
		// X軸の右方向の入力がある場合
		if (UserData::CheckActionKey((int)PlayerActionKey::P_SWAP_MOVE_RIGHT, PRESSED) && oldStick[0]) {
			ResourceManager::StartSound(cursor_se);
			oldStick[0] = false;
			flg = true;
			int snum[4] = { -1,-1,-1,-1 };// オブジェクトのインデックス保存用配列
			int current_color = searchedObj->GetColorData();// 現在の選択オブジェクトの色を取得
			int current_type = searchedObj->object_type;


			// 全オブジェクトを探索
			for (int i = 0;  i < objNum; i++)
			{
				// 他のオブジェクトとの距離が0でない場合に処理を行う
				if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) != 0) {
					int x = (int)searchedObj->GetLocalLocation().x / 40;
					int y = (int)searchedObj->GetLocalLocation().y / 40;

					// 現在の位置の一時保存と、探索中マークの設定
					int tmp = posRelation[y][x];
					posRelation[y][x] = 999;

					// 真横のオブジェクトを探す
					for (int j = x; j < 1280 / 40; j++)
					{
						// 真横に有効なオブジェクトがあり、かつ現在の色と異なる場合
						if (posRelation[y][j] != -1 && posRelation[y][j] != 999) {
							int next_color = searchedObjAll[posRelation[y][j]]->GetColorData();
							int next_type = searchedObjAll[posRelation[y][j]]->object_type;
							if (CheckBlock(current_type, current_color, next_type, next_color, color)) {
								// エネミーは常に選択可能
								snum[0] = posRelation[y][j];
								break;
							}
						}
					}

					//縦横探知
					int h = 0;
					// 真横で見つからなければ上下も探す
					while (snum[0] == -1)
					{
						h++;
						for (int j = x; j < 1280 / 40; j++)
						{
							// 上方向に探索
							if (y - h > -1) {
								if (posRelation[y - h][j] != -1 && posRelation[y][j] != 999) {

									int next_color = searchedObjAll[posRelation[y - h][j]]->GetColorData();
									int next_type = searchedObjAll[posRelation[y - h][j]]->object_type;
									if (CheckBlock(current_type, current_color, next_type, next_color, color)) {
										// エネミーは常に選択可能
										snum[0] = posRelation[y - h][j];
										break;
									}
								}
							}

							if (y + h < 19) {
								// 下方向に探索
								if (posRelation[y + h][j] != -1 && posRelation[y][j] != 999) {

									int next_color = searchedObjAll[posRelation[y + h][j]]->GetColorData();
									int next_type = searchedObjAll[posRelation[y + h][j]]->object_type;
									if (CheckBlock(current_type, current_color, next_type, next_color, color)) {
										// エネミーは常に選択可能
										snum[0] = posRelation[y + h][j];
										break;
									}
								}
							}
						}	
						// 探索範囲を超えたらループ終了
						if (y - h <= -1 && y + h >= 19) {
							break;
						}
					}
					// 見つからなければ元の位置に戻す
					if (snum[0] == -1) {
						snum[0] = tmp;
					}

				}
			}
			// 選択対象が有効であれば一時変数に保存
			if (snum[0] > -1 && snum[0] < 999) {
				objSelectNumTmp = snum[0];
			}
	
		}
		else if (UserData::CheckActionKey((int)PlayerActionKey::P_SWAP_MOVE_LEFT, PRESSED) && oldStick[1]) {
			ResourceManager::StartSound(cursor_se);
			oldStick[1] = false;
			flg = true;


			float nearLen[4] = { 1000.f,1000.f,1000.f,1000.f };
			int snum[4] = { -1,-1,-1,-1 };
			int current_color = searchedObj->GetColorData();
			int current_type = searchedObj->object_type;

			for (int i = 0; i < objNum; i++)
			{
				if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) != 0) {

					int x = (int)searchedObj->GetLocalLocation().x / 40;
					int y = (int)searchedObj->GetLocalLocation().y / 40;

					int tmp = posRelation[y][x];
					posRelation[y][x] = 999;


					//真横探知
					for (int j = x; j > -1; j--)
					{
						if (posRelation[y][j] != -1 && posRelation[y][j] != 999) {

							int next_color = searchedObjAll[posRelation[y][j]]->GetColorData();
							int next_type = searchedObjAll[posRelation[y][j]]->object_type;
							if (CheckBlock(current_type, current_color, next_type, next_color, color)) {
								// エネミーは常に選択可能
								snum[0] = posRelation[y][j];
								break;
							}
						}
					}

					//縦横探知
					int h = 0;
					while (snum[0] == -1)
					{
						h++;
						for (int j = x; j > 0; j--)
						{
							if (y - h > -1) {
								if (posRelation[y - h][j] != -1 && posRelation[y][j] != 999) {

									int next_color = searchedObjAll[posRelation[y - h][j]]->GetColorData();
									int next_type = searchedObjAll[posRelation[y - h][j]]->object_type;
									if (CheckBlock(current_type, current_color, next_type, next_color, color)) {
										// エネミーは常に選択可能
										snum[0] = posRelation[y - h][j];
										break;
									}
								}
							}

							if (y + h < 19) {
								if (posRelation[y + h][j] != -1 && posRelation[y][j] != 999) {

									int next_color = searchedObjAll[posRelation[y + h][j]]->GetColorData();
									int next_type = searchedObjAll[posRelation[y + h][j]]->object_type;
									if (CheckBlock(current_type, current_color, next_type, next_color, color)) {
										// エネミーは常に選択可能
										snum[0] = posRelation[y + h][j];
										break;
									}
								}
							}
						}

						if (y - h <= -1 && y + h >= 19) {
							break;
						}
					}

					if (snum[0] == -1) {
						snum[0] = tmp;
					}
				}
			}

			if (snum[0] > -1 && snum[0] < 999) {
				objSelectNumTmp = snum[0];
			}
		}
		else if (!UserData::CheckActionKey((int)PlayerActionKey::P_SWAP_MOVE_RIGHT, PRESSED) && !UserData::CheckActionKey((int)PlayerActionKey::P_SWAP_MOVE_LEFT, PRESSED)) {
			oldStick[0] = true;
			oldStick[1] = true;
			stickTimer[0] = 0;
		}

		//スティックを倒し続けた場合
		if (!oldStick[0] || !oldStick[1]) {
			stickTimer[0]++;
		}
		if (stickTimer[0] > 15) {
			oldStick[0] = true;
			oldStick[1] = true;
			stickTimer[0] = 0;
		}

		//Y軸
		if (UserData::CheckActionKey((int)PlayerActionKey::P_SWAP_MOVE_UP, PRESSED) && oldStick[2]) {
			ResourceManager::StartSound(cursor_se);
			oldStick[2] = false;
			flg = true;
			int tutirial_num = -1;
			int current_color = searchedObj->GetColorData();
			int current_type = searchedObj->object_type;

			for (int i = 0; i < objNum; i++)
			{
				if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) != 0) {

					int x = (int)searchedObj->GetLocalLocation().x / 40;
					int y = (int)searchedObj->GetLocalLocation().y / 40;

					int tmp = posRelation[y][x];
					posRelation[y][x] = 999;

					//真上探知
					for (int j = y; j > -1; j--)
					{
						if (posRelation[j][x] != -1 && posRelation[j][x] != 999) {
							int next_color = searchedObjAll[posRelation[j][x]]->GetColorData();
							int next_type = searchedObjAll[posRelation[j][x]]->object_type;
							if (CheckBlock(current_type, current_color, next_type, next_color, color)) {
								// エネミーは常に選択可能
								tutirial_num = posRelation[j][x];
								break;
							}
						}
					}

					//縦横探知
					int w = 0;
					while (tutirial_num == -1)
					{
						w++;
						for (int j = y - 1; j > 0; j--)
						{
							if (x - w > -1) {
								if (posRelation[j][x - w] != -1 && posRelation[j][x - w] != 999) {
									int next_color = searchedObjAll[posRelation[j][x - w]]->GetColorData();
									int next_type = searchedObjAll[posRelation[j][x - w]]->object_type;
									if (CheckBlock(current_type, current_color, next_type, next_color, color)) {
										// エネミーは常に選択可能
										tutirial_num = posRelation[j][x - w];
										break;
									}
								}
							}

							if (x + w < 32) {
								if (posRelation[j][x + w] != -1 && posRelation[j][x + w] != 999) {
									int next_color = searchedObjAll[posRelation[j][x + w]]->GetColorData();
									int next_type = searchedObjAll[posRelation[j][x + w]]->object_type;
									if (CheckBlock(current_type, current_color, next_type, next_color, color)) {
										// エネミーは常に選択可能
										tutirial_num = posRelation[j][x + w];
										break;
									}
								}
							}
						}

						if (x - w <= -1 && x + w >= 32) {
							break;
						}
					}

					if (tutirial_num == -1) {
						tutirial_num = tmp;
					}

				}
			}

			if (tutirial_num > -1 && tutirial_num < 999) {
				objSelectNumTmp = tutirial_num;
			}
		}
		else if (UserData::CheckActionKey((int)PlayerActionKey::P_SWAP_MOVE_DOWN, PRESSED) && oldStick[3]) {
			ResourceManager::StartSound(cursor_se); 
			oldStick[3] = false;
			flg = true;
			int tutirial_num = -1;
			int current_color = searchedObj->GetColorData();
			int current_type = searchedObj->object_type;

			for (int i = 0; i < objNum; i++)
			{
				if (GetLength(searchedObj->GetLocalLocation(), searchedObjAll[i]->GetLocalLocation()) != 0) {

					int x = (int)searchedObj->GetLocalLocation().x / 40;
					int y = (int)searchedObj->GetLocalLocation().y / 40;

					int tmp = posRelation[y][x];
					posRelation[y][x] = 999;

					//真上探知
					for (int j = y; j < 720 / 40; j++)
					{
						if (posRelation[j][x] != -1 && posRelation[j][x] != 999) {
							int next_color = searchedObjAll[posRelation[j][x]]->GetColorData();
							int next_type = searchedObjAll[posRelation[j][x]]->object_type;
							if (CheckBlock(current_type, current_color, next_type, next_color, color)) {
								// エネミーは常に選択可能
								tutirial_num = posRelation[j][x];
								break;
							}
						}
					}

					//縦横探知
					int w = 0;
					while (tutirial_num == -1)
					{
						w++;
						for (int j = y + 1; j < 720 / 40; j++)
						{
							if (x - w > -1) {
								if (posRelation[j][x - w] != -1 && posRelation[j][x - w] != 999) {
									int next_color = searchedObjAll[posRelation[j][x - w]]->GetColorData();
									int next_type = searchedObjAll[posRelation[j][x - w]]->object_type;
									if (CheckBlock(current_type, current_color, next_type, next_color, color)) {
										// エネミーは常に選択可能
										tutirial_num = posRelation[j][x - w];
										break;
									}
								}
							}

							if (x + w < 32) {
								if (posRelation[j][x + w] != -1 && posRelation[j][x + w] != 999) {
									int next_color = searchedObjAll[posRelation[j][x + w]]->GetColorData();
									int next_type = searchedObjAll[posRelation[j][x + w]]->object_type;
									if (CheckBlock(current_type, current_color, next_type, next_color, color)) {
										// エネミーは常に選択可能
										tutirial_num = posRelation[j][x + w];
										break;
									}
								}
							}
						}

						if (x - w <= -1 && x + w >= 32) {
							break;
						}
					}

					if (tutirial_num == -1) {
						tutirial_num = tmp;
					}

				}
			}

			if (tutirial_num > -1 && tutirial_num < 999) {
				objSelectNumTmp = tutirial_num;
			}
		}
		else if (!UserData::CheckActionKey((int)PlayerActionKey::P_SWAP_MOVE_UP, PRESSED) && !UserData::CheckActionKey((int)PlayerActionKey::P_SWAP_MOVE_DOWN, PRESSED)) {
			oldStick[2] = true;
			oldStick[3] = true;
			stickTimer[1] = 0;
		}

		//スティックを倒し続けた場合
		if (!oldStick[2] || !oldStick[3]) {
			stickTimer[1]++;
		}
		if (stickTimer[1] > 15) {
			oldStick[2] = true;
			oldStick[3] = true;
			stickTimer[1] = 0;
		}

		searchedObj = searchedObjAll[objSelectNumTmp];
	}
	else if (searchedObjFlg) {//最初
		float len = 1000;
		int n = 0;
		for (int i = 0; i < objNum; i++)
		{
			if (GetLength(local_location, searchedObjAll[i]->GetLocalLocation()) < len) {
				len = GetLength(local_location, searchedObjAll[i]->GetLocalLocation());
				n = i;
			}
		}
		objSelectNumTmp = n;
		searchedObj = searchedObjAll[objSelectNumTmp];
	}
	else {
		searchedObj = nullptr;
	}

}

bool Player::CheckBlock(int current_type, int current_color, int next_type, int next_color, int player_color)
{
	// エネミーとプレイヤーは常に選択可能
	if (next_type == ENEMY || next_type == PLAYER)
	{
		return true;
	}
	// 現在のオブジェクトがブロックで、次のオブジェクトもブロックで、かつプレイヤーと同じ色ならスキップ
	else if (current_type == BLOCK && next_type == BLOCK && next_color == player_color)
	{
		return true;
	}
	// 色が違うなら選択可能
	else if (next_color != player_color)
	{
		return true;
	}


	return false;
}

float Player::GetLength(Vector2D l1, Vector2D l2) 
{
	float len;
	float x, y;

	x = l2.x - l1.x;
	y = l2.y - l1.y;

	len = sqrtf(powf(x, 2) + powf(y, 2));

	return len;
}

void Player::LoadPlayerImage()
{
	//プレイヤーアニメーション格納
	for (int i = 0; i < PLAYER_STATE_NUM; i++)
	{
		player_image[i] = ResourceManager::SetDivGraph(player_imagepath[i], 
			player_anim_image_num[i][0], 
			player_anim_image_num[i][1], 
			player_anim_image_num[i][2], 
			player_anim_image_num[i][3], 
			player_anim_image_num[i][4], 
			player_anim_image_num[i][5],TRUE);
	}

	//プレイヤー正面画像格納
	player_front_image = ResourceManager::SetDivGraph("Resource/Images/sozai/player_posing.PNG", 3, 3, 1, 100, 100, 0);
}

void Player::PlayerSound()
{
	//スローモーション中は10フレームに一回だけ音を鳴らす
	if (searchFlg == FALSE || (searchFlg == TRUE && frame % 10 == 0))
	{
		//プレイヤー歩行
		if (stageHitFlg[1][bottom] == true && velocity.x != 0 && frame % 15 == 0)
		{
			ResourceManager::StartSound(walk_se[now_riding]);
		}
		//ジャンプ
		if (UserData::CheckActionKey((int)PlayerActionKey::P_JUMP, ON_BUTTON) && (damageEffectTime == 90.f || damageEffectTime < 45.f) && ((state == 0 && stageHitFlg[1][bottom]) || state == 1))
		{
			ResourceManager::StartSound(jump_se);
		}

		//ダメージ
		if (damageFlg && !damageOldFlg) {
			ResourceManager::StartSound(damage_se[GetColorNum(this->color)]);
			ResourceManager::StartSound(base_damage_se);
		}
	}
}

void Player::PlayerAnim()
{
	switch (p_state)
	{
	case PlayerState::IDLE_LEFT:
	case PlayerState::IDLE_RIGHT:
		angle[0] = -20.f;
		angle[1] = 20.f;
		angle[2] = 20.f;
		angle[3] = -20.f;
		break;

	case PlayerState::MOVE_LEFT:
	case PlayerState::MOVE_RIGHT:
		float speed;
		speed = abs(velocity.x) * 0.3f;
		if (searchFlg) {
			speed = speed * 0.02f;
		}

		if (p_state != pStateOld) {
			angle[0] = -20.f;
			angle[1] = 20.f;
			angle[2] = 20.f;
			angle[3] = -20.f;
		}

		if (animFlg) {
			angle[0] += speed;
			angle[1] -= speed;
			angle[2] -= speed * 1.5f;
			angle[3] += speed * 1.5f;

			if (angle[0] > 20.f) {
				animFlg = false;
			}
		}
		else {
			angle[0] -= speed;
			angle[1] += speed;
			angle[2] += speed * 1.5f;
			angle[3] -= speed * 1.5f;

			if (angle[0] < -20.f) {
				animFlg = true;
			}
		}

		break;

	case PlayerState::JUMP_LEFT:
	case PlayerState::JUMP_RIGHT:
		if (moveFrontFlg == true) {
			angle[0] = -60.f;
			angle[1] = -60.f;
			angle[2] = 0.f;
			angle[3] = 0.f;
		}
		else {
			angle[0] = 60.f;
			angle[1] = 60.f;
			angle[2] = 0.f;
			angle[3] = 0.f;
		}
		break;

	default:

		break;
	}
}

void Player::AnimStateUpdate()
{
	//右向きかどうか
	if (moveFrontFlg)
	{
		//ダメージを受けていたらダメージアニメーション
		if (damageEffectFlg &&damageEffectTime != 90.f && damageEffectTime > 45.f)
		{
			p_state = PlayerState::DAMAGE_RIGHT;
			return;
		}
		//ジャンプ更新
		if (!stageHitFlg[1][bottom]) 
		{
			p_state = PlayerState::JUMP_RIGHT;
		}
		else
		{
			//idle状態判定
			if (velocity.x > -0.4f && velocity.x < 0.4f) 
			{
				p_state = PlayerState::IDLE_RIGHT;
		
			}
			else
			{
				p_state = PlayerState::MOVE_RIGHT;
			}
		}
	}
	//左向きなら
	else
	{
		//ダメージを受けていたらダメージアニメーション
		if (damageEffectFlg && damageEffectTime != 90.f && damageEffectTime > 45.f)
		{
			p_state = PlayerState::DAMAGE_LEFT;
			return;
		}
		//ジャンプ更新
		if (!stageHitFlg[1][bottom])
		{
			p_state = PlayerState::JUMP_LEFT;
		}
		else
		{
			//idle状態判定
			if (velocity.x > -0.4f && velocity.x < 0.4f)
			{
				p_state = PlayerState::IDLE_LEFT;

			}
			else
			{
				p_state = PlayerState::MOVE_LEFT;
			}
		}
	}
}

void Player::DrawPlayerImage()const
{
	if (emoteFlg || spawn_anim_flg) {
		//プレイヤー正面描画
		DrawGraph(local_location.x, local_location.y, ResourceManager::GetDivGraph(player_front_image, GetColorNum(color)), TRUE);

	}
	else
	{//見た目

	//ダメージ中描画
		if ((p_state == PlayerState::DAMAGE_LEFT || p_state == PlayerState::DAMAGE_RIGHT) && damageEffectTime > 45.f) {
			if ((int)damageEffectTime % 4 != 0) {
				DrawGraph(local_location.x, local_location.y, ResourceManager::GetDivGraph(player_image[p_state], GetColorNum(color)), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 20);
				DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff00000, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}
		}
		//ジャンプ中描画
		else if (p_state ==PlayerState::JUMP_LEFT || p_state == PlayerState::JUMP_RIGHT)
		{
			DrawGraph(local_location.x, local_location.y, ResourceManager::GetDivGraph(player_image[p_state], GetColorNum(color)), TRUE);
		}
		//通常描画
		else
		{
			//ダメージ後の無敵中なら点滅
			if ((int)damageEffectTime % 4 != 0) {
				ResourceManager::DrawColorAnimGraph(local_location + (erea / 2), player_image[p_state], color, false);
			}
		}
	}
}

Vector2D Player::RotationLocation(Vector2D BaseLoc, Vector2D Loc, float r) const
{
	Vector2D l;
	l.x = Loc.x - BaseLoc.x;
	l.y = Loc.y - BaseLoc.y;

	Vector2D ret;
	ret.x = l.x * cosf(r) - l.y * sinf(r);
	ret.y = l.x * sinf(r) + l.y * cosf(r);

	ret.x += BaseLoc.x;
	ret.y += BaseLoc.y;

	return ret;
}

void Player::PlayerReset(ObjectManager* _manager)
{
	//座標をリスポーン地点に設定
	location = _manager->player_respawn;
	//移動量を0にする
	velocity = 0;
	//色交換演出を止める
	swapTimer = -1;
	//色交換中かどうかをリセットする
	searchFlg = false;
	//プレイヤーによって選択されているオブジェクトをリセットする
	_manager->SetNowCurrentObject(nullptr);
	//死亡演出のタイマーをリセットする
	deathTimer = 0;
	damageFlg = false;
	//ダメージ演出関連をリセットする
	damageEffectTime = 90.f;
	damageEffectFlg = false;
	//HPを初期値に戻す
	hp = 5;
	//プレイヤーの色を初期色に戻す
	color = DEFAULT_PLAYER_COLOR;
	//プレイヤー再生成フラグを下ろす
	_manager->player_respawn_flg = false;
	//プレイヤースポーンエフェクトの生成
	//_manager->SpawnEffect({ _manager->player_respawn.x + PLAYER_WIDTH / 2 ,_manager->player_respawn.y + PLAYER_HEIGHT / 2 }, { 20,20 }, PlayerSpawnEffect, 30, _manager->GetPlayerColor());

	//プレイヤースポーン演出の開始
	spawn_anim_flg = true;
	spawn_anim_timer = 0;
}
