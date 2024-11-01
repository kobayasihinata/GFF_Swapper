#include "Player.h"
#include "../Utility/PadInput.h"
#include "../Utility/KeyInput.h"
#include "../Utility/ResourceManager.h"
#include "../Scene/GameMain.h"
#define _USE_MATH_DEFINES
#include <math.h>

Player::Player()
{
	frame = 0;
	type = PLAYER;
	can_swap = TRUE;	//プレイヤーのcan_swapは真でも偽でも大丈夫
	can_hit = TRUE;

	vector = { 0,0 };
	saveVec = vector;

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
	state = 0;
	stateFlg = false;
	fps = 0;
	oldObjNum = 0;
	for (int i = 0; i < 4; i++)
	{
		angle[i] = 0.f;
	}
	pState = idle;
	pStateOld = idle;
	moveFrontFlg = true;
	animFlg = false;	
	circleAng = 0.f;

	landing_se = ResourceManager::SetSound("Resource/Sounds/Player/walk_normal.wav");
	ResourceManager::SetSoundVolume(landing_se, 255);
	walk_se[0] = ResourceManager::SetSound("Resource/Sounds/Player/walk_normal.wav");
	walk_se[1] = ResourceManager::SetSound("Resource/Sounds/Player/walk_fire.wav");
	walk_se[2] = ResourceManager::SetSound("Resource/Sounds/Player/walk_grass.wav");
	walk_se[3] = ResourceManager::SetSound("Resource/Sounds/Player/walk_water.wav");
	jump_se = ResourceManager::SetSound("Resource/Sounds/Player/player_jump.wav");
	damage_se[0] = ResourceManager::SetSound("Resource/Sounds/Player/damage_fire.wav");
	damage_se[1] = ResourceManager::SetSound("Resource/Sounds/Player/damage_grass.wav");
	damage_se[2] = ResourceManager::SetSound("Resource/Sounds/Player/damage_water.wav");
	cursor_se = ResourceManager::SetSound("Resource/Sounds/Player/cursor.wav");

	for (int i = 0; i < 4; i++)
	{
		old_walk_se[i] = walk_se[i];
	}
	old_jump_se = jump_se;
	for (int i = 0; i < 3; i++)
	{
		old_damage_se[i] = damage_se[i];
	}
	old_cursor_se = cursor_se;

	now_riding = 0;
	draw_color = 0;

	emoteFlg = false;
	emoteCnt = 0;
}

Player::~Player()
{

}

void Player::Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)
{
	location = _location;
	erea = _erea;
	color = _color_data;
	draw_color = color;

	object_pos = _object_pos;

	damageFlg = false;
}

void Player::Update(GameMain* _g)
{
	
	if (!is_tutorial) {
		__super::Update(_g);


		//意図しない変更を防止
		SavePlayerSound();

		if (searchFlg) {
			if (fps++ > 6000) {
				fps = 0;
			}
		}
		else {
			fps = 0;
		}

		//移動エフェクト
		if (vector.x != 0 || vector.y != 0)
		{
			_g->SpawnEffect(location, erea, ShineEffect, 20, color);
		}
		//ステージ遷移時に座標だけ移動させる用（体力や色情報などはそのまま）
		if (_g->GetPlayerRespawnFlg())
		{
			location = _g->GetPlayerRespawnLocation();
			_g->SetPlayerRespawnFlg(false);
		}
		if (stageHitFlg[1][bottom] != true) { //重力
			switch (state)
			{
			case 0:
				if (searchFlg) {
					vector.y += 1.f * 0.02f;
				}
				else {
					vector.y += 1.f;
				}
				if (vector.y > 16.f) {
					vector.y = 16.f;
				}
				break;

			case 1:
				if (searchFlg) {
					vector.y += 0.5f * 0.02f;
				}
				else {
					vector.y += 0.5f;
				}
				if (vector.y > 8.f) {
					vector.y = 8.f;
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
				_g->SpawnEffect(location, erea, LandingEffect, 15, color);
				ResourceManager::StartSound(landing_se);
				effect_once = true;
			}
			vector.y = 0.f;
		}

		if (objNum <= 0) {
			searchedObjFlg = false;
		}
		else {
			searchedObjFlg = true;
		}

		oldSearchFlg = searchFlg;
		//Bボタンで色の交換ができるモードと切り替え
		if (PadInput::OnPressed(XINPUT_BUTTON_B) && !_g->GetPauseAfter() && swapTimer < 0) {
			SelectObject();
			searchFlg = true;
		}
		else if (PadInput::OnRelease(XINPUT_BUTTON_B) && searchFlg && searchedObj != nullptr && swapTimer < 0) {
			//交換エフェクトにかかる時間を受け取る
			swapTimer = _g->Swap(this, searchedObj);
			objSelectNumTmp = 0;

			//描画する色を白に
			draw_color = WHITE;
		}
		else if (PadInput::OnRelease(XINPUT_BUTTON_B) && swapTimer < 0) {//交換できるオブジェクトが画面内になかった時
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

		pStateOld = pState;
		if (hp > 0) {
			MoveActor();
		}
		PlayerAnim();

		if (searchFlg) {
			location.x += vector.x * 0.02f;
			location.y += vector.y * 0.02f;
		}
		else {
			location.x += vector.x;
			location.y += vector.y;
		}

		//交換中だけ対象のオブジェクトの見た目を白くする
		if (swapTimer > SWAP_EFFECT_STOP_TIME)
		{
			//選択中のオブジェクトを更新
			_g->SetNowCurrentObject(searchedObj);
		}
		else
		{
			_g->SetNowCurrentObject(nullptr);
		}
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
			if (damageEffectTime == 90) {

				_g->CameraImpact(10);
				_g->SpawnEffect(location, erea, DamageEffect, 20, color);
			}
			damageEffectTime--;
			if (damageEffectTime <= 0) {
				damageEffectFlg = false;
				damageEffectTime = 90;
				damageFlg = false;
				//エフェクトが終わった後に体力が0ならプレイヤーを削除
			}
		}

		if (hp <= 0) {
			damageEffectFlg = false;
			vector.x = 0.f;
			vector.y = 0.f;
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

		now_riding = 0;

		if (circleAng++ >= 360.f) {
			circleAng = 0.f;
		}

		//ゲームオーバー
		if (hp <= 0) {
			deathTimer++;
			if (deathTimer > 90)
			{
				_g->UpdateState(GameMainState::GameOver);
			}
		}
	}

}

void Player::Draw()const
{
	/*SetFontSize(35);
	DrawFormatString(0, 220, 0xff0000, "%0.1f %0.1f", location.x, location.y);*/

	if(hp <= 0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (deathTimer * 2));
	}

	if (emoteFlg) {
		DrawPlayerFront(false);
	}
	else {
		if (damageEffectFlg) {
			if (damageEffectTime % 10 == 0) {
				DrawPlayer();
			}
		}
		else {//見た目
			DrawPlayer();
		}
	}

	DrawPlayerFront(true);
	for (int i = 0; i < hp; i++)
	{
		ResourceManager::DrawHeart({ 150.f + i * 50.f,75.f }, { 40.f,40.f });
	}

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
}

void Player::Finalize()
{

}

void Player::Hit(Object* _object)
{
	__super::Hit(_object);

	//ブロックと当たった時の処理
	if (
			(_object->GetObjectType() == BLOCK && _object->GetCanHit() == TRUE)||
			(_object->GetObjectType() == FIRE && _object->GetCanSwap() == TRUE && this->color == RED) ||
			(_object->GetObjectType() == WOOD && _object->GetCanSwap() == TRUE && this->color == GREEN)||
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
				vector.x = 0.f;
				move[left] = t;
			}
		}

		//右方向に埋まらないようにする
		if (stageHitFlg[0][right]) {//右方向に埋まっていたら
			float t = _object->GetLocation().x - (location.x + erea.x);
			if (t != 0) {
				vector.x = 0.f;
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

	//ダメージ
	if (!damageEffectFlg && CheckCollision(_object->GetLocation(), _object->GetErea()) && _object->GetCanHit()) {
		//色ごとの判定
		switch (color)
		{
		case RED:
			if (_object->GetObjectType() == WATER || _object->GetColorData() == BLUE) {
				damageFlg = true;
			}
			
			break;

		case BLUE:
			if (_object->GetObjectType() == WOOD || _object->GetColorData() == GREEN) {
				damageFlg = true;
			}
			
			break;

		case GREEN:
			if (_object->GetObjectType() == FIRE || _object->GetColorData() == RED) {
				damageFlg = true;
			}
			
			break;

		default:
			break;
		}
	}

	//ダメージゾーンを上書きする
	if ((_object->GetObjectType() == WATER && _object->GetCanSwap() == FALSE && this->color == GREEN) ||
		(_object->GetObjectType() == FIRE && _object->GetCanSwap() == FALSE && this->color == BLUE) ||
		(_object->GetObjectType() == WOOD && _object->GetCanSwap() == FALSE && this->color == RED))
	{
		if (!_object->GetIsBossAttack())
		{
			_object->SetColorData(color);
		}
	}

	//ジャンプの奴
	if (((_object->GetObjectType() == WATER && _object->GetCanSwap() == FALSE && this->color == BLUE && !stateFlg) ||
		(_object->GetObjectType() == FIRE && _object->GetCanSwap() == FALSE && this->color == RED && !stateFlg) ||
		(_object->GetObjectType() == WOOD && _object->GetCanSwap() == FALSE && this->color == GREEN && !stateFlg)) && !_object->GetIsBossAttack()){
		state = 1;
		stateFlg = true;
	}
	else {
		if (!stateFlg) {
			state = 0;
		}
	}

	//自分が乗っている(触れている)ブロックに応じてSEを変える
	if ((_object->GetObjectType() == FIRE && this->color != RED) || _object->GetObjectType() == WOOD || _object->GetObjectType() == WATER)
	{
		now_riding = _object->GetObjectType() - 2;
	}
	else if(_object->GetObjectType() == FIRE && this->color == RED)
	{
		now_riding = 0;
	}

	//ボスの木攻撃に当たった時、プレイヤーを跳ねさせる
	if (_object->GetObjectType() == BLOCK && _object->GetIsBossAttack() == TRUE && _object->GetLocation().y > this->location.y)
	{
		vector.y = -20;
	}

	//チュートリアル
	if (_object->GetObjectType() == TUTORIAL)
	{
		is_tutorial = true;
	}
}

void Player::MoveActor()
{
	//ジャンプ
	if ((PadInput::OnButton(XINPUT_BUTTON_A)/*|| PadInput::OnPressed(XINPUT_BUTTON_A)*/)) {
		switch (state)
		{
		case 0:
			if (stageHitFlg[1][bottom]) {
				vector.y = -23.f;
			}
			break;

		case 1:
			vector.y = -23.f;

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
			vector = saveVec;
		}

		if (PadInput::TipLeftLStick(STICKL_X) > 0.1f) {
			stick = PadInput::TipLeftLStick(STICKL_X);
			vector.x += stick * 1.f;
			if (vector.x > 7.5f) {
				vector.x = 7.5f;
			}
			pState = moving;
			moveFrontFlg = true;
		}
		else if (PadInput::TipLeftLStick(STICKL_X) < -0.1f) {
			stick = PadInput::TipLeftLStick(STICKL_X);
			vector.x += stick * 1.f;
			if (vector.x < -7.5f) {
				vector.x = -7.5f;
			}
			pState = moving;
			moveFrontFlg = false;
		}
		else {
			if (vector.x > 0.1f) {
				vector.x += -0.2f;
			}
			else if (vector.x < 0.1f) {
				vector.x += 0.2f;
			}

			pState = moving;

			if (vector.x > -0.4f && vector.x < 0.4f) {
				vector.x = 0.f;
				pState = idle;
			}
		}
	}
	else {
		if (!oldSearchFlg) {
			saveVec = vector;
		}
	}

	if (stageHitFlg[1][bottom] != true) {
		pState = jump;
	}
}

bool Player::SearchColor(Object* ob)
{
	if (ob != nullptr && ob->GetColorData() > 0 && ob != this){
		if(ob->GetLocalLocation().x >= 0 && ob->GetLocalLocation().x <= 1280 && ob->GetLocalLocation().y >= 0 && ob->GetLocalLocation().y <= 720) {
			searchedObjAll[objNum] = ob;
			int x = (int)ob->GetLocalLocation().x / 40;
			int y = (int)ob->GetLocalLocation().y / 40;
			posRelation[y][x] = objNum;
			objNum++;
		}
	}
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
		if ((PadInput::TipLeftLStick(STICKL_X) > 0.8f || PadInput::OnPressed(XINPUT_BUTTON_DPAD_RIGHT)) && oldStick[0]) {
			ResourceManager::StartSound(cursor_se);
			oldStick[0] = false;
			flg = true;
			int snum[4] = { -1,-1,-1,-1 };// オブジェクトのインデックス保存用配列
			int current_color = searchedObj->GetColorData();// 現在の選択オブジェクトの色を取得
			
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
							int next_type = searchedObjAll[posRelation[y][j]]->GetObjectType();
							if (next_color != current_color || next_type == ENEMY) {
								// 選択対象を更新
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
									/*if (next_color != current_color) {
										snum[0] = posRelation[y - h][j];
										break;
									}*/
									int next_type = searchedObjAll[posRelation[y - h][j]]->GetObjectType();
									if (next_color != current_color || next_type == ENEMY) {
										// 選択対象を更新
										snum[0] = posRelation[y - h][j];
										break;
									}
									
								}
							}

							if (y + h < 19) {
								// 下方向に探索
								if (posRelation[y + h][j] != -1 && posRelation[y][j] != 999) {

									int next_color = searchedObjAll[posRelation[y + h][j]]->GetColorData();
									/*if (next_color != current_color) {
										snum[0] = posRelation[y + h][j];
										break;
									}*/
									int next_type = searchedObjAll[posRelation[y + h][j]]->GetObjectType();
									if (next_color != current_color || next_type == ENEMY) {
										// 選択対象を更新
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
		else if ((PadInput::TipLeftLStick(STICKL_X) < -0.8f || PadInput::OnPressed(XINPUT_BUTTON_DPAD_LEFT)) && oldStick[1]) {
			ResourceManager::StartSound(cursor_se);
			oldStick[1] = false;
			flg = true;


			float nearLen[4] = { 1000.f,1000.f,1000.f,1000.f };
			int snum[4] = { -1,-1,-1,-1 };
			int current_color = searchedObj->GetColorData();
			
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
							/*if (next_color != current_color) {
								snum[0] = posRelation[y][j];
								break;
							}*/
							int next_type = searchedObjAll[posRelation[y][j]]->GetObjectType();
							if (next_color != current_color || next_type == ENEMY) {
								// 選択対象を更新
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
									/*if (next_color != current_color) {
										snum[0] = posRelation[y - h][j];
										break;
									}*/
									int next_type = searchedObjAll[posRelation[y - h][j]]->GetObjectType();
									if (next_color != current_color || next_type == ENEMY) {
										// 選択対象を更新
										snum[0] = posRelation[y - h][j];
										break;
									}
								}
							}

							if (y + h < 19) {
								if (posRelation[y + h][j] != -1 && posRelation[y][j] != 999) {

									int next_color = searchedObjAll[posRelation[y + h][j]]->GetColorData();
									/*if (next_color != current_color) {
										snum[0] = posRelation[y + h][j];
										break;
									}*/
									int next_type = searchedObjAll[posRelation[y + h][j]]->GetObjectType();
									if (next_color != current_color || next_type == ENEMY) {
										// 選択対象を更新
										snum[0] = posRelation[y - h][j];
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
		else if ((PadInput::TipLeftLStick(STICKL_X) < 0.1f && PadInput::TipLeftLStick(STICKL_X) > -0.1f) && (!PadInput::OnPressed(XINPUT_BUTTON_DPAD_RIGHT) && !PadInput::OnPressed(XINPUT_BUTTON_DPAD_LEFT))) {
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
		if ((PadInput::TipLeftLStick(STICKL_Y) > 0.8f || PadInput::OnPressed(XINPUT_BUTTON_DPAD_UP)) && oldStick[2]) {
			ResourceManager::StartSound(cursor_se);
			oldStick[2] = false;
			flg = true;
			int tutirial_num = -1;
			int current_color = searchedObj->GetColorData();

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
							/*if (next_color != current_color) {
								tutirial_num = posRelation[j][x];
								break;
							}*/
							int next_type = searchedObjAll[posRelation[j][x]]->GetObjectType();
							if (next_color != current_color || next_type == ENEMY) {
								// 選択対象を更新
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
									/*if (next_color != current_color) {
										tutirial_num = posRelation[j][x - w];
										break;
									}*/
									int next_type = searchedObjAll[posRelation[j][x - w]]->GetObjectType();
									if (next_color != current_color || next_type == ENEMY) {
										// 選択対象を更新
										tutirial_num = posRelation[j][x - w];
										break;
									}
								}
							}

							if (x + w < 32) {
								if (posRelation[j][x + w] != -1 && posRelation[j][x + w] != 999) {
									int next_color = searchedObjAll[posRelation[j][x + w]]->GetColorData();
									/*if (next_color != current_color) {
										tutirial_num = posRelation[j][x + w];
										break;
									}*/
									int next_type = searchedObjAll[posRelation[j][x + w]]->GetObjectType();
									if (next_color != current_color || next_type == ENEMY) {
										// 選択対象を更新
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
		else if ((PadInput::TipLeftLStick(STICKL_Y) < -0.8f || PadInput::OnPressed(XINPUT_BUTTON_DPAD_DOWN)) && oldStick[3]) {
			ResourceManager::StartSound(cursor_se); 
			oldStick[3] = false;
			flg = true;
			int tutirial_num = -1;
			int current_color = searchedObj->GetColorData();

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
							/*if (next_color != current_color) {
								tutirial_num = posRelation[j][x];
								break;
							}*/
							int next_type = searchedObjAll[posRelation[j][x]]->GetObjectType();
							if (next_color != current_color || next_type == ENEMY) {
								// 選択対象を更新
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
									/*if (next_color != current_color) {
										tutirial_num = posRelation[j][x - w];
										break;
									}*/
									int next_type = searchedObjAll[posRelation[j][x - w]]->GetObjectType();
									if (next_color != current_color || next_type == ENEMY) {
										// 選択対象を更新
										tutirial_num = posRelation[j][x - w];
										break;
									}
								}
							}

							if (x + w < 32) {
								if (posRelation[j][x + w] != -1 && posRelation[j][x + w] != 999) {
									int next_color = searchedObjAll[posRelation[j][x + w]]->GetColorData();
									/*if (next_color != current_color) {
										tutirial_num = posRelation[j][x + w];
										break;
									}*/
									int next_type = searchedObjAll[posRelation[j][x + w]]->GetObjectType();
									if (next_color != current_color || next_type == ENEMY) {
										// 選択対象を更新
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
		else if ((PadInput::TipLeftLStick(STICKL_Y) < 0.1f && PadInput::TipLeftLStick(STICKL_Y) > -0.1f) && (!PadInput::OnPressed(XINPUT_BUTTON_DPAD_UP) && !PadInput::OnPressed(XINPUT_BUTTON_DPAD_DOWN))) {
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
		oldObjNum = 0;
	}
	else {
		searchedObj = nullptr;
	}
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

void Player::PlayerSound()
{
	//スローモーション中は10フレームに一回だけ音を鳴らす
	if (searchFlg == FALSE || (searchFlg == TRUE && frame % 10 == 0))
	{
		//プレイヤー歩行
		if (stageHitFlg[1][bottom] == true && vector.x != 0 && frame % 15 == 0)
		{
			ResourceManager::StartSound(walk_se[now_riding]);
		}
		//ジャンプ
		if (PadInput::OnButton(XINPUT_BUTTON_A) == true && ((state == 0 && stageHitFlg[1][bottom]) || state == 1))
		{
			ResourceManager::StartSound(jump_se);
		}

		//ダメージ
		if (damageFlg && !damageOldFlg && now_riding > 0) {
			ResourceManager::StartSound(damage_se[now_riding-1]);
		}
	}
}

void Player::PlayerAnim()
{
	switch (pState)
	{
	case idle:
		angle[0] = -20.f;
		angle[1] = 20.f;
		angle[2] = 20.f;
		angle[3] = -20.f;
		break;

	case moving:
		float speed;
		speed = abs(vector.x) * 0.3f;
		if (searchFlg) {
			speed = speed * 0.02f;
		}

		if (pState != pStateOld) {
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

	case jump:
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

void Player::DrawPlayer() const
{
	if (moveFrontFlg == true) {
		//腕
		if (hp > 4) {
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 50, 28, 7, local_location.x + 35, local_location.y + 50, angle[0], draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 50, 28, 7, local_location.x + 35, local_location.y + 50, angle[0], 0x000000, false);
		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 50, 28, 7, local_location.x + 35, local_location.y + 50, angle[0], 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 50, 28, 7, local_location.x + 35, local_location.y + 50, angle[0], 0xffffff, false);
		}
		if (hp > 0) {
			//頭
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 80 + 55, 23, 15, local_location.x + 30, local_location.y + 80, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 80 + 55, 23, 15, local_location.x + 30, local_location.y + 80, 0, 0x000000, false);

			//目
			ResourceManager::DrawRotaBox(local_location.x + 30 - 6, local_location.y + 80 + 56, 6, 7, local_location.x + 30, local_location.y + 80, 0, 0x000000, true);

			//首
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 80 + 42, 10, 5, local_location.x + 30, local_location.y + 80, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 80 + 42, 10, 5, local_location.x + 30, local_location.y + 80, 0, 0x000000, false);

			//胴体
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 80 + 37 / 2, 21, 37, local_location.x + 30, local_location.y + 80, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 80 + 37 / 2, 21, 37, local_location.x + 30, local_location.y + 80, 0, 0x000000, false);

			//バッグ
			ResourceManager::DrawRotaBox(local_location.x + 30 + 15, local_location.y + 80 + 20, 5, 23, local_location.x + 30, local_location.y + 80, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 30 + 15, local_location.y + 80 + 20, 5, 23, local_location.x + 30, local_location.y + 80, 0, 0x000000, false);
			ResourceManager::DrawRotaBox(local_location.x + 30 + 15, local_location.y + 80 + 20, 3, 15, local_location.x + 30, local_location.y + 80, 0, 0x000000, true);
		}
		else {
			//頭
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 76, 23, 15, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 76, 23, 15, local_location.x, local_location.y, 0, 0xffffff, false);

			//目
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) - 6, local_location.y - (erea.y) + 76, 6, 7, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) - 6, local_location.y - (erea.y) + 76, 6, 7, local_location.x, local_location.y, 0, 0xffffff, false);

			//首
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 62, 10, 5, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 62, 10, 5, local_location.x, local_location.y, 0, 0xffffff, false);

			//胴体
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 37, 21, 37, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 37, 21, 37, local_location.x, local_location.y, 0, 0xffffff, false);

			//バッグ
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) + 15, local_location.y - (erea.y) + 40, 5, 23, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) + 15, local_location.y - (erea.y) + 40, 5, 23, local_location.x, local_location.y, 0, 0xffffff, false);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) + 15, local_location.y - (erea.y) + 40, 3, 15, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) + 15, local_location.y - (erea.y) + 40, 3, 15, local_location.x, local_location.y, 0, 0xffffff, false);
		}

		//腕
		if (hp > 3) {
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 55, 28, 7, local_location.x + 35, local_location.y + 55, angle[1], draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 55, 28, 7, local_location.x + 35, local_location.y + 55, angle[1], 0x000000, false);
		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 55, 28, 7, local_location.x + 35, local_location.y + 55, angle[1], 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x + 25, local_location.y + 55, 28, 7, local_location.x + 35, local_location.y + 55, angle[1], 0xffffff, false);
		}


		//足 前から
		if (hp > 2) {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], 0x000000, false);

		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], 0xffffff, false);
		}
		if (hp > 1) {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], 0x000000, false);

		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], 0xffffff, false);
		}

		if (hp > 1) {
			//帽子　中央
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, draw_color, true);
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0x000000, false);
			//帽子　右側
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, draw_color, true);
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0x000000, false);
			//帽子　左側
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, draw_color, true);
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0x000000, false);
		}
		else {
			//帽子　中央
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0x000000, true);
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0xffffff, false);
			//帽子　右側
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0x000000, true);
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0xffffff, false);
			//帽子　左側
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0x000000, true);
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0xffffff, false);
		}
	}
	else {
		if (hp > 3) {
			ResourceManager::DrawRotaBox(local_location.x + 15, local_location.y + 50, 28, 7, local_location.x + 25, local_location.y + 50, angle[0] + 180, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 15, local_location.y + 50, 28, 7, local_location.x + 25, local_location.y + 50, angle[0] + 180, 0x000000, false);
		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 15, local_location.y + 50, 28, 7, local_location.x + 25, local_location.y + 50, angle[0] + 180, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x + 15, local_location.y + 50, 28, 7, local_location.x + 25, local_location.y + 50, angle[0] + 180, 0xffffff, false);

		}
		if (hp > 0) {
			//頭
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 76, 23, 15, local_location.x, local_location.y, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 76, 23, 15, local_location.x, local_location.y, 0, 0x000000, false);

			//目
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) + 6, local_location.y - (erea.y) + 76, 6, 7, local_location.x, local_location.y, 0, 0x000000, true);

			//首
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 62, 10, 5, local_location.x, local_location.y, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 62, 10, 5, local_location.x, local_location.y, 0, 0x000000, false);

			//胴体
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 37, 21, 37, local_location.x, local_location.y, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 37, 21, 37, local_location.x, local_location.y, 0, 0x000000, false);

			//バッグ
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) - 15, local_location.y - (erea.y) + 40, 5, 23, local_location.x, local_location.y, 0, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) - 15, local_location.y - (erea.y) + 40, 5, 23, local_location.x, local_location.y, 0, 0x000000, false);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) - 15, local_location.y - (erea.y) + 40, 3, 15, local_location.x, local_location.y, 0, 0x000000, true);
		}
		else {
			//頭
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 76, 23, 15, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 76, 23, 15, local_location.x, local_location.y, 0, 0xffffff, false);

			//目
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) - 6, local_location.y - (erea.y) + 76, 6, 7, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) - 6, local_location.y - (erea.y) + 76, 6, 7, local_location.x, local_location.y, 0, 0xffffff, false);

			//首
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 62, 10, 5, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 62, 10, 5, local_location.x, local_location.y, 0, 0xffffff, false);

			//胴体
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 37, 21, 37, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2), local_location.y - (erea.y) + 37, 21, 37, local_location.x, local_location.y, 0, 0xffffff, false);

			//バッグ
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) + 15, local_location.y - (erea.y) + 40, 5, 23, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) + 15, local_location.y - (erea.y) + 40, 5, 23, local_location.x, local_location.y, 0, 0xffffff, false);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) + 15, local_location.y - (erea.y) + 40, 3, 15, local_location.x, local_location.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x - (erea.x / 2) + 15, local_location.y - (erea.y) + 40, 3, 15, local_location.x, local_location.y, 0, 0xffffff, false);
		}
		//腕

		if (hp > 4) {
			ResourceManager::DrawRotaBox(local_location.x + 15, local_location.y + 55, 28, 7, local_location.x + 25, local_location.y + 55, angle[1] + 180, draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 15, local_location.y + 55, 28, 7, local_location.x + 25, local_location.y + 55, angle[1] + 180, 0x000000, false);
		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 15, local_location.y + 55, 28, 7, local_location.x + 25, local_location.y + 55, angle[1] + 180, 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x + 15, local_location.y + 55, 28, 7, local_location.x + 25, local_location.y + 55, angle[1] + 180, 0xffffff, false);
		}


		//足 前から
		if (hp > 2) {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], 0x000000, false);

		}
		else {

			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[2], 0xffffff, false);

		}
		if (hp > 1) {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], draw_color, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], 0x000000, false);

		}
		else {
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], 0x000000, true);
			ResourceManager::DrawRotaBox(local_location.x + 30, local_location.y + 70, 7, 27, local_location.x + 30, local_location.y + 80, angle[3], 0xffffff, false);
		}

		if (hp > 1) {
			//帽子　中央
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, draw_color, true);
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0x000000, false);
			//帽子　右側
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, draw_color, true);
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0x000000, false);
			//帽子　左側
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, draw_color, true);
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0x000000, false);

		}
		else {
			//帽子　中央
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0x000000, true);
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 20, local_location.y + 20, local_location.x + 40, local_location.y + 20, 0xffffff, false);
			//帽子　右側
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0x000000, true);
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 40, local_location.y + 20, local_location.x + 52, local_location.y + 15, 0xffffff, false);
			//帽子　左側
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0x000000, true);
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, local_location.x + 8, local_location.y + 15, local_location.x + 20, local_location.y + 20, 0xffffff, false);

		}

	}
}

void Player::DrawPlayerFront(bool f) const
{
	Vector2D uiL;
	
	if (f) {
		uiL.x = 45.f;
		uiL.y = 45.f;
		DrawCircleAA(uiL.x + (erea.x / 2), uiL.y + (erea.y / 2), 65.f, 64, 0x555555, TRUE);
		DrawCircleAA(uiL.x + (erea.x / 2), uiL.y + (erea.y / 2), 65.f, 64, 0xffffff, FALSE, 4.f);
		if (hp > 0) {
			//頭
			ResourceManager::DrawRotaBox(uiL.x + 30, uiL.y + 80 + 55, 23, 15, uiL.x + 30, uiL.y + 80, 0, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 30, uiL.y + 80 + 55, 23, 15, uiL.x + 30, uiL.y + 80, 0, 0x000000, false);

			//目
			ResourceManager::DrawRotaBox(uiL.x + 30 - 6, uiL.y + 80 + 56, 6, 7, uiL.x + 30, uiL.y + 80, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x - 30 + 6, uiL.y - 100 + 76, 6, 7, uiL.x, uiL.y, 0, 0x000000, true);


			//首
			ResourceManager::DrawRotaBox(uiL.x + 30, uiL.y + 80 + 42, 10, 5, uiL.x + 30, uiL.y + 80, 0, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 30, uiL.y + 80 + 42, 10, 5, uiL.x + 30, uiL.y + 80, 0, 0x000000, false);

			//胴体
			ResourceManager::DrawRotaBox(uiL.x + 30, uiL.y + 80 + 37 / 2, 21, 37, uiL.x + 30, uiL.y + 80, 0, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 30, uiL.y + 80 + 37 / 2, 21, 37, uiL.x + 30, uiL.y + 80, 0, 0x000000, false);
		}
		else {
			//頭
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2), uiL.y - (erea.y) + 76, 23, 15, uiL.x, uiL.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2), uiL.y - (erea.y) + 76, 23, 15, uiL.x, uiL.y, 0, 0xffffff, false);

			//目
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2) - 6, uiL.y - (erea.y) + 76, 6, 7, uiL.x, uiL.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2) - 6, uiL.y - (erea.y) + 76, 6, 7, uiL.x, uiL.y, 0, 0xffffff, false);

			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2) + 6, uiL.y - (erea.y) + 76, 6, 7, uiL.x, uiL.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2) + 6, uiL.y - (erea.y) + 76, 6, 7, uiL.x, uiL.y, 0, 0xffffff, false);

			//首
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2), uiL.y - (erea.y) + 62, 10, 5, uiL.x, uiL.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2), uiL.y - (erea.y) + 62, 10, 5, uiL.x, uiL.y, 0, 0xffffff, false);

			//胴体
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2), uiL.y - (erea.y) + 37, 21, 37, uiL.x, uiL.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2), uiL.y - (erea.y) + 37, 21, 37, uiL.x, uiL.y, 0, 0xffffff, false);
		}
		//左腕
		if (hp > 4) {
			ResourceManager::DrawRotaBox(uiL.x + 4, uiL.y + 50, 28, 7, uiL.x - 6, uiL.y + 50, 180, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 4, uiL.y + 50, 28, 7, uiL.x - 6, uiL.y + 50, 180, 0x000000, false);
		}
		else {
			ResourceManager::DrawRotaBox(uiL.x + 4, uiL.y + 50, 28, 7, uiL.x - 6, uiL.y + 50, 180, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x + 4, uiL.y + 50, 28, 7, uiL.x - 6, uiL.y + 50, 180, 0xffffff, false);

		}
		//腕
		if (hp > 3) {
			ResourceManager::DrawRotaBox(uiL.x + 34, uiL.y + 50, 28, 7, uiL.x + 45, uiL.y + 50, 0, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 34, uiL.y + 50, 28, 7, uiL.x + 45, uiL.y + 50, 0, 0x000000, false);
		}
		else {
			ResourceManager::DrawRotaBox(uiL.x + 34, uiL.y + 50, 28, 7, uiL.x + 45, uiL.y + 50, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x + 34, uiL.y + 50, 28, 7, uiL.x + 45, uiL.y + 50, 0, 0xffffff, false);
		}


		//足 前から
		if (hp > 2) {
			ResourceManager::DrawRotaBox(uiL.x + 25, uiL.y + 70, 7, 27, uiL.x + 25, uiL.y + 83, 0, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 25, uiL.y + 70, 7, 27, uiL.x + 25, uiL.y + 83, 0, 0x000000, false);

		}
		else {
			ResourceManager::DrawRotaBox(uiL.x + 25, uiL.y + 70, 7, 27, uiL.x + 25, uiL.y + 83, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x + 25, uiL.y + 70, 7, 27, uiL.x + 25, uiL.y + 83, 0, 0xffffff, false);
		}
		if (hp > 1) {
			ResourceManager::DrawRotaBox(uiL.x + 35, uiL.y + 70, 7, 27, uiL.x + 35, uiL.y + 83, 0, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 35, uiL.y + 70, 7, 27, uiL.x + 35, uiL.y + 83, 0, 0x000000, false);

		}
		else {
			ResourceManager::DrawRotaBox(uiL.x + 35, uiL.y + 70, 7, 27, uiL.x + 35, uiL.y + 83, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x + 35, uiL.y + 70, 7, 27, uiL.x + 35, uiL.y + 83, 0, 0xffffff, false);
		}

		if (hp > 1) {
			//帽子　中央
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 20, uiL.y + 20, uiL.x + 40, uiL.y + 20, draw_color, true);
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 20, uiL.y + 20, uiL.x + 40, uiL.y + 20, 0x000000, false);
			//帽子　右側
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 40, uiL.y + 20, uiL.x + 52, uiL.y + 15, draw_color, true);
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 40, uiL.y + 20, uiL.x + 52, uiL.y + 15, 0x000000, false);
			//帽子　左側
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 8, uiL.y + 15, uiL.x + 20, uiL.y + 20, draw_color, true);
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 8, uiL.y + 15, uiL.x + 20, uiL.y + 20, 0x000000, false);
		}
		else {
			//帽子　中央
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 20, uiL.y + 20, uiL.x + 40, uiL.y + 20, 0x000000, true);
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 20, uiL.y + 20, uiL.x + 40, uiL.y + 20, 0xffffff, false);
			//帽子　右側
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 40, uiL.y + 20, uiL.x + 52, uiL.y + 15, 0x000000, true);
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 40, uiL.y + 20, uiL.x + 52, uiL.y + 15, 0xffffff, false);
			//帽子　左側
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 8, uiL.y + 15, uiL.x + 20, uiL.y + 20, 0x000000, true);
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 8, uiL.y + 15, uiL.x + 20, uiL.y + 20, 0xffffff, false);
		}
	}
	else {
		uiL = local_location;
		if (hp > 0) {
			//頭
			ResourceManager::DrawRotaBox(uiL.x + 30, uiL.y + 80 + 55, 23, 15, uiL.x + 30, uiL.y + 80, 0, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 30, uiL.y + 80 + 55, 23, 15, uiL.x + 30, uiL.y + 80, 0, 0x000000, false);

			//目
			ResourceManager::DrawRotaBox(uiL.x + 30 - 6, uiL.y + 80 + 56, 6, 7, uiL.x + 30, uiL.y + 80, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2) + 6, uiL.y - (erea.y) + 76, 6, 7, uiL.x, uiL.y, 0, 0x000000, true);


			//首
			ResourceManager::DrawRotaBox(uiL.x + 30, uiL.y + 80 + 42, 10, 5, uiL.x + 30, uiL.y + 80, 0, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 30, uiL.y + 80 + 42, 10, 5, uiL.x + 30, uiL.y + 80, 0, 0x000000, false);

			//胴体
			ResourceManager::DrawRotaBox(uiL.x + 30, uiL.y + 80 + 37 / 2, 21, 37, uiL.x + 30, uiL.y + 80, 0, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 30, uiL.y + 80 + 37 / 2, 21, 37, uiL.x + 30, uiL.y + 80, 0, 0x000000, false);
		}
		else {
			//頭
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2), uiL.y - (erea.y) + 76, 23, 15, uiL.x, uiL.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2), uiL.y - (erea.y) + 76, 23, 15, uiL.x, uiL.y, 0, 0xffffff, false);

			//目
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2) - 6, uiL.y - (erea.y) + 76, 6, 7, uiL.x, uiL.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2) - 6, uiL.y - (erea.y) + 76, 6, 7, uiL.x, uiL.y, 0, 0xffffff, false);

			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2) + 6, uiL.y - (erea.y) + 76, 6, 7, uiL.x, uiL.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2) + 6, uiL.y - (erea.y) + 76, 6, 7, uiL.x, uiL.y, 0, 0xffffff, false);

			//首
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2), uiL.y - (erea.y) + 62, 10, 5, uiL.x, uiL.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2), uiL.y - (erea.y) + 62, 10, 5, uiL.x, uiL.y, 0, 0xffffff, false);

			//胴体
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2), uiL.y - (erea.y) + 37, 21, 37, uiL.x, uiL.y, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x - (erea.x / 2), uiL.y - (erea.y) + 37, 21, 37, uiL.x, uiL.y, 0, 0xffffff, false);
		}
		//左腕
		if (hp > 4) {
			ResourceManager::DrawRotaBox(uiL.x + 4, uiL.y + 50, 28, 7, uiL.x + 15, uiL.y + 50, 120, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 4, uiL.y + 50, 28, 7, uiL.x + 15, uiL.y + 50, 120, 0x000000, false);
		}
		else {
			ResourceManager::DrawRotaBox(uiL.x + 4, uiL.y + 50, 28, 7, uiL.x + 15, uiL.y + 50, 120, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x + 4, uiL.y + 50, 28, 7, uiL.x + 15, uiL.y + 50, 120, 0xffffff, false);

		}
		//腕
		if (hp > 3) {
			ResourceManager::DrawRotaBox(uiL.x + 34, uiL.y + 50, 28, 7, uiL.x + 45, uiL.y + 50, 300, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 34, uiL.y + 50, 28, 7, uiL.x + 45, uiL.y + 50, 300, 0x000000, false);
		}
		else {
			ResourceManager::DrawRotaBox(uiL.x + 34, uiL.y + 50, 28, 7, uiL.x + 45, uiL.y + 50, 300, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x + 34, uiL.y + 50, 28, 7, uiL.x + 45, uiL.y + 50, 300, 0xffffff, false);
		}


		//足 前から
		if (hp > 2) {
			ResourceManager::DrawRotaBox(uiL.x + 25, uiL.y + 70, 7, 27, uiL.x + 25, uiL.y + 83, 60, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 25, uiL.y + 70, 7, 27, uiL.x + 25, uiL.y + 83, 60, 0x000000, false);

		}
		else {
			ResourceManager::DrawRotaBox(uiL.x + 25, uiL.y + 70, 7, 27, uiL.x + 25, uiL.y + 83, 60, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x + 25, uiL.y + 70, 7, 27, uiL.x + 25, uiL.y + 83, 60, 0xffffff, false);
		}
		if (hp > 1) {
			ResourceManager::DrawRotaBox(uiL.x + 35, uiL.y + 70, 7, 27, uiL.x + 35, uiL.y + 83, 0, draw_color, true);
			ResourceManager::DrawRotaBox(uiL.x + 35, uiL.y + 70, 7, 27, uiL.x + 35, uiL.y + 83, 0, 0x000000, false);

		}
		else {
			ResourceManager::DrawRotaBox(uiL.x + 35, uiL.y + 70, 7, 27, uiL.x + 35, uiL.y + 83, 0, 0x000000, true);
			ResourceManager::DrawRotaBox(uiL.x + 35, uiL.y + 70, 7, 27, uiL.x + 35, uiL.y + 83, 0, 0xffffff, false);
		}

		if (hp > 1) {
			//帽子　中央
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 20, uiL.y + 20, uiL.x + 40, uiL.y + 20, draw_color, true);
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 20, uiL.y + 20, uiL.x + 40, uiL.y + 20, 0x000000, false);
			//帽子　右側
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 40, uiL.y + 20, uiL.x + 52, uiL.y + 15, draw_color, true);
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 40, uiL.y + 20, uiL.x + 52, uiL.y + 15, 0x000000, false);
			//帽子　左側
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 8, uiL.y + 15, uiL.x + 20, uiL.y + 20, draw_color, true);
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 8, uiL.y + 15, uiL.x + 20, uiL.y + 20, 0x000000, false);
		}
		else {
			//帽子　中央
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 20, uiL.y + 20, uiL.x + 40, uiL.y + 20, 0x000000, true);
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 20, uiL.y + 20, uiL.x + 40, uiL.y + 20, 0xffffff, false);
			//帽子　右側
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 40, uiL.y + 20, uiL.x + 52, uiL.y + 15, 0x000000, true);
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 40, uiL.y + 20, uiL.x + 52, uiL.y + 15, 0xffffff, false);
			//帽子　左側
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 8, uiL.y + 15, uiL.x + 20, uiL.y + 20, 0x000000, true);
			DrawTriangleAA(uiL.x + (erea.x / 2), uiL.y, uiL.x + 8, uiL.y + 15, uiL.x + 20, uiL.y + 20, 0xffffff, false);
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

void Player::SavePlayerSound()
{
	for (int i = 0; i < 4; i++)
	{
		if (old_walk_se[i] != walk_se[i])
		{
			walk_se[i] = old_walk_se[i];
		}
	}
	if (old_jump_se != jump_se)
	{
		jump_se = old_jump_se;
	}
	for (int i = 0; i < 3; i++)
	{
		if (old_damage_se[i] != damage_se[i])
		{
			damage_se[i] = old_damage_se[i];
		}
	}
	if (old_cursor_se != cursor_se)
	{
		cursor_se = old_cursor_se;
	}
}

