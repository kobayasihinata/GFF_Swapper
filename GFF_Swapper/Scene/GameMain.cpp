#include "GameMain.h"
#include "Dxlib.h"
#include "../Utility/PadInput.h"
#include "../Utility/KeyInput.h"
#include "../Utility/common.h"
#include"../Utility/ResourceManager.h"
#include "EditScene.h"
#include <fstream>
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Title.h"
#include "End.h"
#include "Help.h"


static Vector2D camera_location = { 0,0 };	//カメラの座標
static const Vector2D screen_origin = { 0,0 };	//カメラの初期位置

GameMain::GameMain(int _stage) :frame(0), impact(0), stage_data{ 0 }, now_stage(0), object_num(0), stage_width_num(0), stage_height_num(0), stage_width(0), stage_height(0), camera_x_lock_flg(true), camera_y_lock_flg(true), x_pos_set_once(false), y_pos_set_once(false), player_object(0), boss_object(0), weather(0), weather_timer(0), move_object_num(0), boss_blind_flg(false), boss_blind_timer(0), player_flg(false), player_respawn_flg(false), fadein_flg(true), create_once(false),pause_after_flg(false), cursor(0), clear_timer(0), set_sound_once(false), gm_state(GameMainState::S_GameMain), now_scene(this), blackout(0)
{
	now_stage = _stage;
}

GameMain::~GameMain()
{

}

void GameMain::Initialize()
{

	weather = new WeatherManager();
	weather->Initialize();

	effect_spawner = new EffectSpawner();
	effect_spawner->Initialize();

	bgm_title =  ResourceManager::SetSound("Resource/Sounds/BGM/Title.wav");
	bgm_normal = ResourceManager::SetSound("Resource/Sounds/BGM/GameMainNormal.wav");
	bgm_noise =  ResourceManager::SetSound("Resource/Sounds/BGM/GameMainNoise.wav");
	bgm_abnormal = ResourceManager::SetSound("Resource/Sounds/BGM/GameMainAbnormal.wav");

	game_clear_bgm = ResourceManager::SetSound("Resource/Sounds/BGM/GameClear.wav");

	cursor_se = ResourceManager::SetSound("Resource/Sounds/Player/cursor.wav");
	decision_se = ResourceManager::SetSound("Resource/Sounds/System/decision.wav");

	back_ground = new BackGround();

	SetStage(now_stage, false);

	back_ground->Initialize({ (float)stage_width,(float)stage_height });

	lock_pos[0].x = 0;
	lock_pos[0].y = 0;
	lock_pos[1].x = stage_width - SCREEN_WIDTH;
	lock_pos[1].y = stage_height - SCREEN_HEIGHT;

	SetWindowIconID(102);

	test_image = LoadGraph("Resource/Images/sozai/grow.PNG");
	test_image2 = LoadGraph("Resource/Images/sozai/ground.PNG");
	
	blackout = 255;
}

void GameMain::Finalize()
{
	//BGMを止める
	ResourceManager::StopSound(bgm_normal);
	ResourceManager::StopSound(bgm_noise);
	ResourceManager::StopSound(bgm_abnormal);

	for (int i = 0; object[i] != nullptr; i++)
	{
		//生成済みのオブジェクトを削除
		object[i]->Finalize();
		delete object[i];
	}
	weather->Finalize();
	delete weather;

	effect_spawner->Finalize();
	delete effect_spawner;

	back_ground->Finalize();
	delete back_ground;

}

AbstractScene* GameMain::Update()
{
	//フレーム測定
	frame++;
	//カメラの更新
	UpdateCamera();

	//演出の終了
	if ((frame > FADEIN_TIME || PadInput::OnRelease(XINPUT_BUTTON_B)) && fadein_flg == true)
	{
		fadein_flg = false;
	}
	//演出中は更新を止める
	if (frame == 1 || fadein_flg == false)
	{
		switch (gm_state)
		{
		case GameMainState::S_GameMain:	//ゲーム本編
			UpdateGameMain();
		break;
		case GameMainState::Pause:	//一時停止
			UpdatePause();
			break;
		case GameMainState::S_Help:	//ヘルプ
			UpdateHelp();
			break;
		case GameMainState::GameClear:	//ゲームクリア
			UpdateGameClear();
			break;
		case GameMainState::Check:
			UpdateCheck();
			break;
		case GameMainState::GameOver:	//ゲームオーバー
			UpdateGameOver();
			break;
		default:
			printfDx("存在しないゲームモード");
			break;
		}
		if (KeyInput::OnKey(KEY_INPUT_C))
		{
			gm_state = GameMainState::GameClear;
		}

#ifdef _DEBUG
		//強制クリア
		if (KeyInput::OnKey(KEY_INPUT_V)) 
		{
			gm_state = GameMainState::GameClear;
		}
		//ステージをいじるシーンへ遷移
		if (KeyInput::OnPresed(KEY_INPUT_E) && KeyInput::OnPresed(KEY_INPUT_D))
		{
			return new EditScene(now_stage);
		}

		if (KeyInput::OnKey(KEY_INPUT_1))
		{
			SetStage(0, false);
		}
		if (KeyInput::OnKey(KEY_INPUT_2))
		{
			SetStage(1, false);
		}
		if (KeyInput::OnKey(KEY_INPUT_3))
		{
			SetStage(2, false);
		}
#endif
	}
		return now_scene;
}

void GameMain::Draw() const
{
	switch (gm_state)
	{
	case GameMainState::S_GameMain:
		DrawGameMain();
		break;
	case GameMainState::Pause:
		DrawGameMain();
		DrawPause();
		break;
	case GameMainState::S_Help:
		DrawGameMain();
		DrawHelp();
		break;
	case GameMainState::GameClear:
		DrawGameMain();
		DrawGameClear();
		break;
	case GameMainState::Check:
		DrawGameMain();
		DrawGameOver();
		DrawCheck();
		break;
	case GameMainState::GameOver:
		DrawGameMain();
		DrawGameOver();
		break;
	}

#ifdef _DEBUG
	SetFontSize(12);
	DrawBox(90, 90, 400, 200, 0x000000, true);
	DrawFormatString(100, 100, 0xffffff, "Object数:%d", object_num);
	DrawFormatString(100, 120, 0xffffff, "Updeteが呼ばれているObject数:%d", in_screen_object.size());

	//DrawFormatString(100, 140, 0xffffff, "normal:%d", 255 - (int)(camera_location.x / 100));
	//DrawFormatString(100, 160, 0xffffff, "noise:%d", (int)(camera_location.x / 100));
	//チュートリアル表示テスト
	SetFontSize(35);
	//DrawFormatString(0, 160, 0xff0000, "%0.1f x %0.1f y",  camera_location.x + KeyInput::GetMouseCursor().x, stage_height - KeyInput::GetMouseCursor().y - camera_location.y);
#endif
}

int GameMain::CreateObject(Object* _object, Vector2D _location, Vector2D _erea, int _color_data)
{
	for (int i = 0; i < OBJECT_NUM; i++)
	{
		//空いている場所に格納する
		if (object[i] == nullptr)
		{
			object[i] = _object;
			object[i]->Initialize(_location, _erea, _color_data, i);
			//プレイヤーの配列上の位置格納
			if (object[i]->GetObjectType() == PLAYER)
			{
				player_object = i;
				player_flg = true;
			}
			if (object[i]->GetObjectType() == BOSS)
			{
				boss_object = i;
			}
			//if (_erea.y == 1200.f)
			//{
			//	boss_attack[attack_num++] = i;
			//}
			object_num++;
			return i;
		}
	}
	return -1;
}

void GameMain::DeleteObject(int i, Object* _object)
{
	if (object[i] != nullptr && _object != nullptr && i >= 0)
	{
		int a;
		a = 0;
		
		//プレイヤーが消されたなら
		if (i == player_object)
		{
			//player_objectをリセット
			player_object = 0;
		}
		//ボスが消されたなら
		if (i == boss_object)
		{
			//boss_objectをリセット
			boss_object = 0;
		}
		//オブジェクトを前に寄せる
		for (int j = i; object[j] != nullptr; j++)
		{
			object[j] = nullptr;
			object[j] = object[j + 1];
			if (object[j] != nullptr)
			{
				object[j]->SetObjectPos(j);
				if (object[j]->GetObjectType() == PLAYER)
				{
					player_object = j;
				}
				if (object[j]->GetObjectType() == BOSS)
				{
					boss_object = j;
				}
			}
			else
			{
				break;
			}
		}
		object_num--;
	}
}

void GameMain::UpdateCamera()
{
	//カメラ更新（ボスステージ）
	if (now_stage == 2)
	{
		camera_location = { 160,40 };
		return;
	}

	//カメラ座標更新
	camera_location.x = object[player_object]->GetCenterLocation().x - (SCREEN_WIDTH / 2);
	camera_location.y = object[player_object]->GetCenterLocation().y - (SCREEN_HEIGHT / 2);

	//X座標が画面左端以下なら
	if (object[player_object]->GetCenterLocation().x < (SCREEN_WIDTH / 2))
	{
		//カメラのX座標を左端固定する
		camera_location.x = lock_pos[0].x;
	}
	//X座標が画面右端以上なら
	if (object[player_object]->GetCenterLocation().x > stage_width - (SCREEN_WIDTH / 2))
	{
		//カメラのX座標を右端固定する
		camera_location.x = lock_pos[1].x;
	}
	//Y座標が画面上端以上なら
	if (object[player_object]->GetCenterLocation().y < (SCREEN_HEIGHT / 2))
	{
		//カメラのX座標を上端固定する
		camera_location.y = lock_pos[0].y;
	}
	//Y座標が画面下端以上なら
	if (object[player_object]->GetCenterLocation().y > stage_height - (SCREEN_HEIGHT / 2) - 10)
	{
		//カメラのX座標を下端固定する
		camera_location.y = lock_pos[1].y;
	}

	//カメラ振動処理
	if (impact > 0)
	{
		impact--;
		impact_rand =(GetRand(impact) - (impact / 2));
	}
	else
	{
		impact = 0;
		impact_rand = 0;
	}
}

void GameMain::LoadStageData(int _stage)
{
	const char* a = "Resource/Dat/1stStageData.txt";
	switch (_stage)
	{
	case 0:
		a = "Resource/Dat/TutorialStageData.txt";
		break;
	case 1:
		a = "Resource/Dat/1stStageData.txt";
		break;
	case 2:
		a = "Resource/Dat/BossStageData.txt";
		break;
	default:
		break;
	}

	std::ifstream file(a);
	//ファイルが読み込めていたなら
	if (file)
	{
		file >> stage_width_num;
		file >> stage_height_num;

		stage_width = stage_width_num * BOX_WIDTH;
		stage_height = stage_height_num * BOX_HEIGHT;
		//配列データを読み込む
		for (int i = 0; i < stage_height_num; i++)
		{
			for (int j = 0; j < stage_width_num; j++)
			{
				file >> stage_data[i][j];
			}
		}
	}
}

void GameMain::SetStage(int _stage, bool _delete_player)
{
	boss_blind_flg = false;
	object_num = 0;

	//すべてのオブジェクトを削除
	DeleteAllObject(_delete_player);

	now_stage = _stage;

	//背景クラスのステージも変更
	back_ground->SetNowStage(now_stage);

	//ファイルの読込
	LoadStageData(now_stage);

	//デフォルトのプレイヤーリスポーン地点の設定
	player_respawn = { (float)100,(float)stage_height - 200 };

	for (int i = stage_height_num - 1; i >= 0; i--)
	{
		for (int j = 0; j < stage_width_num; j++)
		{	
			switch (stage_data[i][j])
			{
			case NULL_BLOCK:
				break;
			case WHITE_BLOCK:
			case GRAY_BLOCK:
			case RED_BLOCK:
			case GREEN_BLOCK:
			case BLUE_BLOCK:
			case FIRE_BLOCK:
			case WOOD_BLOCK:
			case WATER_BLOCK:
			case PLAYER_RESPAWN_BLOCK:
			case WEATHER_NORMAL:
			case WEATHER_RAIN:
			case WEATHER_FIRE:
			case WEATHER_SEED:
				//ステージ内ブロックを生成＆生成位置を格納
				stage_block_pos[i][j] = CreateObject(new Stage(stage_data[i][j],stage_height), { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { BOX_WIDTH ,BOX_HEIGHT }, stage_data[i][j]);
				break;
			case TUTOSTAGE_TRANSITION:
			case FIRSTSTAGE_TRANSITION:
			case BOSSSTAGE_TRANSITION:
				//遷移ブロックを生成＆生成位置を格納
				stage_block_pos[i][j] = CreateObject(new Stage(stage_data[i][j], 0 , stage_data[i][j]), { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { BOX_WIDTH ,BOX_HEIGHT }, stage_data[i][j]);
				break;
			case PLAYER_BLOCK:
				//プレイヤーリスポーン地点の設定
				player_respawn = { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT };
				//エフェクトの生成
				effect_spawner->SpawnEffect({ player_respawn.x + PLAYER_WIDTH / 2 ,player_respawn.y + PLAYER_HEIGHT / 2 }, { 20,20 }, PlayerSpawnEffect, 30, DEFAULT_PLAYER_COLOR);
				break;
			case ENEMY_DEER_RED:
			case ENEMY_DEER_GREEN:
			case ENEMY_DEER_BLUE:
				//鹿の生成
				CreateObject(new EnemyDeer, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 100,100 }, ColorList[stage_data[i][j] - 11]);
				break;
			case ENEMY_BAT_RED:
			case ENEMY_BAT_GREEN:
			case ENEMY_BAT_BLUE:
				//コウモリの生成
				CreateObject(new EnemyBat, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 118,75 }, ColorList[stage_data[i][j] - 14]);
				break;
			case ENEMY_FROG_RED:
			case ENEMY_FROG_GREEN:
			case ENEMY_FROG_BLUE:
				//カエルの生成
				CreateObject(new EnemyFrog, {(float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT}, {50,50}, ColorList[stage_data[i][j] - 17]);
				break;
			case ENEMY_BOSS:
				//ボスの生成
				CreateObject(new Boss, { (float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT }, { 250,250 }, ColorList[stage_data[i][j] - 20]);
				break;
				//チュートリアル開始範囲
			case TUTORIAL_RANGE:
				CreateObject(new Tutorial, {(float)j * BOX_WIDTH ,(float)i * BOX_HEIGHT}, {50, 250}, stage_data[i][j]);
			default:
				break;
			}
		}
	}

	//プレイヤーが居るなら
	if (player_flg)
	{
		//リスポーンフラグを立てる
		player_respawn_flg = true;
	}

	//プレイヤーが生成されていない、もしくは再生成が必要なら
	if (!player_flg || _delete_player)
	{
		//プレイヤーの生成
		CreateObject(new Player, player_respawn, { PLAYER_WIDTH,PLAYER_HEIGHT }, DEFAULT_PLAYER_COLOR);
	}

	//周辺ステージデータの格納
	SetStageBlockAround();

	//壁生成フラグリセット
	create_once = false;
	//カメラのリセット
	ResetCamera();
	//BGMの再生
	if (now_stage == 0)
	{
		ResourceManager::StopSound(bgm_normal);
		ResourceManager::StopSound(bgm_noise);
		ResourceManager::StopSound(bgm_abnormal);

		ResourceManager::StartSound(bgm_normal, TRUE);
		ResourceManager::StartSound(bgm_noise, TRUE);
	}
	else if (now_stage == 2)
	{
		ResourceManager::StopSound(bgm_normal);
		ResourceManager::StopSound(bgm_noise);
		//ResourceManager::StopSound(bgm_abnormal);

		boss_blind_flg = true;
	}
}

void GameMain::ResetCamera()
{
	camera_location = screen_origin;
}

bool GameMain::GetSearchFlg()
{
	return object[player_object]->GetSearchFlg();
}

Vector2D GameMain::GetPlayerLocation()
{
	return object[player_object]->GetLocation();
}

Vector2D GameMain::GetPlayerErea()
{
	return object[player_object]->GetErea();
}

Vector2D GameMain::GetCameraLocation()
{
	return camera_location;
}

void GameMain::CameraImpact(int _num)
{
	impact = _num;
}

void GameMain::SpawnEffect(Vector2D _location, Vector2D _erea, int _type, int _time, int _color,float _angle)
{
	effect_spawner->SpawnEffect(_location, _erea, _type, _time, _color, _angle);
}

int GameMain::Swap(Object* _object1, Object* _object2)
{
	return effect_spawner->Swap(_object1, _object2);
}

bool GameMain::CheckInScreen(Object* _object)const
{
	//画面内に居るか判断 画面端の敵がすり抜けないように地面の更新範囲を広めに
	if (_object != nullptr &&
		(
			(_object->GetObjectType() != ENEMY && 
			 _object->GetLocation().x > camera_location.x - _object->GetErea().x - 80 &&
		     _object->GetLocation().x < camera_location.x + SCREEN_WIDTH + _object->GetErea().x + 80 &&
		     _object->GetLocation().y > camera_location.y - _object->GetErea().y - 80 &&
		     _object->GetLocation().y < camera_location.y + SCREEN_HEIGHT + _object->GetErea().y + 160
		    )
			||
		    (_object->GetObjectType() == ENEMY &&
		     _object->GetLocation().x > camera_location.x - _object->GetErea().x&&
		     _object->GetLocation().x < camera_location.x + SCREEN_WIDTH + _object->GetErea().x &&
		     _object->GetLocation().y > camera_location.y - _object->GetErea().y&&
		     _object->GetLocation().y < camera_location.y + SCREEN_HEIGHT + _object->GetErea().y
		     ) 
		)
	   )
	{
		return true;
	}
	return false;
}

Vector2D GameMain::GetBossLocation()
{
	return object[boss_object]->GetLocation();
}

void GameMain::DeleteAllObject(bool _player_delete)
{
	for (int i = 0; i < OBJECT_NUM; i++)
	{
		//プレイヤー以外削除
		if (i != player_object)
		{
			object[i] = nullptr;
		}
	}
	if (_player_delete)
	{
		object[player_object] = nullptr;
		player_flg = false;
	}
}

void GameMain::PlayerUpdate()
{
	
	//プレイヤーが居ないなら(DeleteObjectされていた、もしくはobject[player_object]がプレイヤーではないなら)
	if (object[player_object] == nullptr || object[player_object]->GetObjectType() != PLAYER)
	{
		//プレイヤーの生成
		CreateObject(new Player, player_respawn, { PLAYER_HEIGHT,PLAYER_WIDTH }, GREEN);
	}
	
	//プレイヤーの更新＆色探知用
	if (object[player_object] != nullptr)
	{
		object[player_object]->SetScreenPosition(camera_location, impact_rand);
		object[player_object]->Update(this);
		move_object_num++;

		for (int i = 0; object[i] != nullptr; i++)
		{
			if (object[i]->GetCanSwap() == TRUE && object[i]->GetObjectType() != PLAYER && boss_blind_flg == false) {
				object[player_object]->SearchColor(object[i]);
			}
			
			//各オブジェクトとの当たり判定
			if (CheckInScreen(object[i]) && object[i]->HitBox(object[player_object]))
			{
				object[i]->Hit(object[player_object]);
				object[player_object]->Hit(object[i]);
			}
		}

		//プレイヤーが落下したときに死亡判定とする
		if (GetPlayerLocation().y > stage_height + 100)
		{
			//ステージとプレイヤーをリセット
			//SetStage(now_stage, true);

			gm_state = GameMainState::GameOver;
		}
	}

}

void GameMain::BossUpdate()
{
	//ボスをスローモーションにしないならコメント解除
	//if (object[boss_object] != nullptr)
	//{
	//	object[boss_object]->SetScreenPosition(camera_location,(GetRand(impact) - (impact / 2)));
	//	object[boss_object]->Update(this);
	//	move_object_num++;
	//	for (int i = 0; object[i] != nullptr; i++)
	//	{
	//		if (object[i]->GetCanSwap() == TRUE && object[i]->GetObjectType() != PLAYER) {
	//			object[player_object]->SearchColor(object[i]);
	//		}
	//		//各オブジェクトとの当たり判定
	//		if (object[i]->HitBox(object[boss_object]))
	//		{
	//			object[i]->Hit(object[boss_object]);
	//			object[boss_object]->Hit(object[i]);
	//		}
	//	}
	//}

	//for (int i = 0; i < attack_num; i++)
	//{
	//	if (object[boss_attack[i]] != nullptr)
	//	{
	//		object[boss_attack[i]]->Update(this);
	//		move_object_num++;
	//	}
	//}
}

void GameMain::SetNowCurrentObject(Object* _object)
{
	now_current_object = _object;
}

Vector2D GameMain::RotationLocation(Vector2D BaseLoc, Vector2D Loc, float r) const
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

void GameMain::UpdateGameMain()
{
	if (PadInput::OnButton(XINPUT_BUTTON_START) && gm_state != GameMainState::Pause)
	{
		gm_state = GameMainState::Pause;
	}

	if (PadInput::OnRelease(XINPUT_BUTTON_B) && pause_after_flg)
	{
		pause_after_flg = false;
	}
	//BGM音量更新
	ResourceManager::SetSoundVolume(bgm_normal, 255 - (int)(camera_location.x / 100));
	ResourceManager::SetSoundVolume(bgm_noise, (int)(camera_location.x / 100));
	//リセット
	move_object_num = 0;
	//各オブジェクトの更新
	if (object[player_object]->GetSearchFlg() == FALSE || (object[player_object]->GetSearchFlg() == TRUE && frame % 10 == 0))
	{
		tutorial_text.Update(camera_location, GetPlayerLocation(), stage_height);
		for (int i = 0; object[i] != nullptr; i++)
		{
			//ローカル座標の更新
			object[i]->SetScreenPosition(camera_location, impact_rand);
		}
		//スクリーン内オブジェクトの更新
		InScreenUpdate();

		for (const auto& in_screen_object : in_screen_object)
		{
			//プレイヤーとボス以外の画面内オブジェクトの更新
			if (
				(
				 (
					in_screen_object == object[boss_object] && boss_blind_flg == false) || 
					in_screen_object != object[boss_object]) &&
				in_screen_object != object[player_object]
				)
			{
				in_screen_object->Update(this);
				move_object_num++;
				for (const auto& in_screen_object2 : this->in_screen_object)
				{
					//各オブジェクトとの当たり判定
					if (in_screen_object->HitBox(in_screen_object2) &&
						in_screen_object2 != object[player_object])
					{
						in_screen_object->Hit(in_screen_object2);
					}
				}
				//プレイヤーに選択されているオブジェクトなら、描画色を変える
				if (in_screen_object != nullptr &&
					in_screen_object == now_current_object &&
					now_current_object != object[boss_object])
				{
					in_screen_object->SetDrawColor(WHITE);
				}
				else if (in_screen_object != nullptr)
				{
					in_screen_object->SetDrawColor(in_screen_object->GetColorData());
				}
			}
		}
		weather->Update(this);
	}

	//プレイヤーの更新
	PlayerUpdate();

	//ボスの更新
	BossUpdate();

	//管理クラスの更新
	effect_spawner->Update(this);

	////背景の更新
	back_ground->Update();

	//プレイヤーがボスエリアに入ったら退路を閉じる
	if (now_stage == 2 && object[player_object]->GetLocalLocation().x > 160 && object[player_object]->GetLocalLocation().x < 200 && create_once == false)
	{
		CreateObject(new Stage(2), { 160,520 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
		CreateObject(new Stage(2), { 160,560 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
		CreateObject(new Stage(2), { 160,600 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
		CreateObject(new Stage(2), { 160,640 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
		CreateObject(new Stage(1), { 120,520 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
		CreateObject(new Stage(1), { 120,560 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
		CreateObject(new Stage(1), { 120,600 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
		CreateObject(new Stage(1), { 120,640 }, { BOX_WIDTH,BOX_HEIGHT }, 0);
		ResourceManager::StartSound(bgm_abnormal, TRUE);
		boss_blind_timer = 10;
		boss_blind_flg = false;
		create_once = true;
	}

	//一定時間暗転
	if (--boss_blind_timer < 0)
	{
		boss_blind_timer = 0;
	}

	//ボスステージに遷移
	if (now_stage != 2 && object[player_object]->GetLocation().x > stage_width - 100 && object[player_object]->GetLocation().y > stage_height - 300)
	{
		SetStage(2, false);
		boss_blind_flg = true;
	}

	
}

void GameMain::DrawGameMain()const
{
	//背景の描画
	back_ground->Draw(camera_location);

	//for (int i = 0; i < attack_num; i++)
	//{
	//	object[boss_attack[i]]->Draw();
	//}

	// チュートリアルテキストはボスエリアで描画しない
	if (now_stage != 2)
	{
		tutorial_text.Draw();
	}

	int enemy_pos[32];
	int tutirial_num = 0;

	//オブジェクトの描画
	for (int i = 0; object[i] != nullptr; i++)
	{
		if (player_object == i) {
			continue;
		}
		if (CheckInScreen(object[i]) == true)
		{
			if (boss_blind_flg == true)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 - sqrtf(powf(fabsf(object[player_object]->GetLocation().x - object[i]->GetLocation().x), 2) + powf(fabsf(object[player_object]->GetLocation().y - object[i]->GetLocation().y), 2))));
				object[i]->Draw();
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}
			else
			{
				if (object[i]->GetObjectType() == ENEMY) 
				{
					enemy_pos[tutirial_num] = i;
					tutirial_num++;
				}
				else 
				{
					object[i]->Draw();
				}

			}
		}
	}

	//エフェクトの描画
	effect_spawner->Draw();

	//天気管理クラスの描画
	weather->Draw();

	//敵描画
	for (int i = 0; i < tutirial_num; i++)
	{
		object[enemy_pos[i]]->Draw();
	}

	//プレイヤーを最後に描画
	object[player_object]->Draw();

	//暗転
	if (boss_blind_timer > 0)
	{
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
	}
	//フェードイン演出
	if (fadein_flg == true)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (frame * (255 / FADEIN_TIME) + 3));
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

void GameMain::UpdatePause()
{
	//BGM音量更新
	ResourceManager::SetSoundVolume(bgm_normal, 155 - (int)(camera_location.x / 100));
	ResourceManager::SetSoundVolume(bgm_noise, (int)(camera_location.x / 100) - 50);
	ResourceManager::SetSoundVolume(bgm_abnormal, 100);
	cursorOld = cursor;

	if (PadInput::TipLeftLStick(STICKL_X) < -0.5f && move_cursor_once == false)
	{
		if (--cursor < 0)
		{
			cursor = 2;
		}
		ResourceManager::StartSound(cursor_se);
		move_cursor_once = true;
	}
	else if (PadInput::TipLeftLStick(STICKL_X) > 0.5f && move_cursor_once == false)
	{
		if (++cursor > 2)
		{
			cursor = 0;
		}
		ResourceManager::StartSound(cursor_se);
		move_cursor_once = true;
	}
	else if(PadInput::TipLeftLStick(STICKL_X) < 0.2f && PadInput::TipLeftLStick(STICKL_X) > -0.2f)
	{
		move_cursor_once = false;
	}
	if (PadInput::OnButton(XINPUT_BUTTON_DPAD_LEFT))
	{
		if (--cursor < 0)
		{
			cursor = 2;
		}
		ResourceManager::StartSound(cursor_se);
	}
	if (PadInput::OnButton(XINPUT_BUTTON_DPAD_RIGHT))
	{
		if (++cursor > 2)
		{
			cursor = 0;
		}
		ResourceManager::StartSound(cursor_se);
	}

	if (PadInput::OnButton(XINPUT_BUTTON_B) || PadInput::OnButton(XINPUT_BUTTON_START)) {
		if (cursor == 0)
		{
			ResourceManager::StartSound(decision_se);
			gm_state = GameMainState::S_GameMain;
			pause_after_flg = true;
		}
		else if(cursor == 1)
		{
			ResourceManager::StartSound(decision_se);
			gm_state = GameMainState::S_Help;
		}
		else
		{
			ResourceManager::StartSound(decision_se);
			cursor = 0;
			before_check_scene = gm_state;
			gm_state = GameMainState::Check;
		}
	}


	if (circleAng++ >= 360.f) {
		circleAng = 0.f;
	}
}

void GameMain::DrawPause()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawBoxAA(200, 410, 500, 510, 0x000000, TRUE);
	DrawBoxAA(200, 410, 500, 510, 0xffffff, FALSE);

	DrawBoxAA(480, 410, 780, 510, 0x000000, TRUE);
	DrawBoxAA(480, 410, 780, 510, 0xffffff, FALSE);

	DrawBoxAA(780, 410, 1080, 510, 0x000000, TRUE);
	DrawBoxAA(780, 410, 1080, 510, 0xffffff, FALSE);

	int fontsize = 192 / 2;
	SetFontSize(fontsize * 2);
	DrawString(400, 100, "P", 0xff0000);
	DrawString(400 + fontsize * 1, 100, "A", 0xffffff);
	DrawString(400 + fontsize * 2, 100, "U", 0x00ff00);
	DrawString(400 + fontsize * 3, 100, "S", 0xffffff);
	DrawString(400 + fontsize * 4, 100, "E", 0x0000ff);

	SetFontSize(48);
	DrawString(300, 436, "BACK", 0xffffff);
	DrawString(580, 436, "Help", 0xffffff);
	DrawString(860, 436, "TITLE", 0xffffff);

	Vector2D circleLoc;

	if (cursor == 0) {
		circleLoc.x = 350.f;
		circleLoc.y = 460.f;
	}
	else if(cursor == 1) {
		circleLoc.x = 640.f;
		circleLoc.y = 460.f;
	}
	else
	{
		circleLoc.x = 930.f;
		circleLoc.y = 460.f;
	}

	DrawCircleAA(circleLoc.x, circleLoc.y, 40.f * 2.3f, 40, 0xffff00, FALSE, 4.f * 2.3f);

	Vector2D base;
	base.x = circleLoc.x;
	base.y = circleLoc.y;

	Vector2D l[3];
	l[0].x = base.x;
	l[0].y = base.y - 40.f * 2.3f;

	l[0] = RotationLocation(base, l[0], (float)(circleAng * M_PI / 180));

	l[1] = RotationLocation(base, l[0], (float)(120.f * M_PI / 180));

	l[2] = RotationLocation(base, l[0], (float)(240.f * M_PI / 180));


	DrawCircleAA(l[0].x, l[0].y, 15.f * 1.5f, 32, 0xcc0000, TRUE);
	DrawCircleAA(l[1].x, l[1].y, 15.f * 1.5f, 32, 0x3c78d8, TRUE);
	DrawCircleAA(l[2].x, l[2].y, 15.f * 1.5f, 32, 0x6aa84f, TRUE);
}

void GameMain::UpdateHelp()
{
	if (PadInput::OnButton(XINPUT_BUTTON_B))
	{
		gm_state = GameMainState::Pause;
	}
}

void GameMain::DrawHelp()const
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	SetFontSize(34);

	// 操作方法
	DrawBoxAA(20, 32, 552,682, 0xffffff, FALSE, 2.0f);
	DrawBoxAA(22, 34, 550, 680, 0x3f3a40, TRUE, 2.0f);

	//左スティック：移動
	DrawCircleAA(80.f, 90.f, 50.f, 32, 0x1c2b3e, TRUE);
	DrawCircleAA(80.f, 90.f, 30.f, 32, 0x525156, TRUE);
	DrawString(150, 70, "LeftStick : Move", 0xffffff);

	//Aボタン：ジャンプ
	DrawCircleAA(80.f, 195.f, 50.f, 32, 0x1c2b3e);
	DrawCircleAA(80.f, 195.f, 30.f, 32, 0x557b0e);
	DrawString(150, 178, "A Button : Jump", 0xffffff);	

	//Bボタン(長押し)：色の交換
	DrawCircleAA(80.f, 300.f, 50.f, 32, 0x1c2b3e);
	DrawCircleAA(80.f, 300.f, 30.f, 32, 0xc5482c);
	DrawString(145, 260, "B Button\n(LongPress)", 0xffffff);	
	DrawString(350, 278, ":ColorSwap", 0xffffff);

	//STARTボタン : PAUSE
	DrawCircleAA(73.f, 395.f, 38.f, 100, 0x1c2b3e, TRUE);
	DrawCircleAA(173.f, 395.f, 38.f, 100, 0x1c2b3e, TRUE);
	DrawBoxAA(60.f, 358.f, 180.f, 433.f, 0x1c2b3e, TRUE);
	DrawString(220, 375, "START Button:Porse", 0xffffff);

	//B(長押し中) + 十字カーソル
	DrawCircleAA(80.f, 490.f, 50.f, 32, 0x1c2b3e);
	DrawCircleAA(80.f, 490.f, 30.f, 32, 0xc5482c);
	DrawCircleAA(231.f, 490.f, 50.f, 32, 0x1c2b3e);
	DrawBox(218, 448, 242, 533, 0x211d1e, TRUE);
	DrawBox(190, 480, 272, 505, 0x211d1e, TRUE);
	DrawString(300, 435, "B(Pressing)\nD-Pad\n:ColorSelect", 0xffffff);

	//B(長押し中) + 左スティック
	DrawCircleAA(80.f, 605.f, 50.f, 32, 0x1c2b3e);
	DrawCircleAA(80.f, 605.f, 30.f, 32, 0xc5482c);
	DrawCircleAA(231.f, 605.f, 50.f, 32, 0x1c2b3e);
	DrawCircleAA(231.f, 605.f, 30.f, 32, 0x525156);
	DrawString(300, 560, "B(Pressing)\nLeftStick\n:ColorSelect", 0xffffff);

	SetFontSize(49);
	DrawString(68, 170, "A", 0xffffff);
	DrawString(68, 275, "B", 0xffffff);
	DrawString(58, 370, "START", 0xffffff);
	DrawString(68, 465, "B", 0xffffff);
	DrawString(130, 465, "＋", 0xffffff);
	DrawString(68, 580, "B", 0xffffff);
	DrawString(130, 580, "＋", 0xffffff);

	// 属性表
	SetFontSize(34);
	DrawBoxAA(710, 32, 1252, 452, 0xffffff, FALSE, 2.0f);
	DrawBoxAA(712, 34, 1250, 450, 0x3f3a40, TRUE, 2.0f);

	//赤 (火属性)
	DrawCircleAA(980.f, 100.f, 60.f, 32, 0xcc0000, TRUE);
	DrawString(930, 65, " Red\n(Fire)", 0xFFFF55);
	DrawString(1090, 180, "Damage", 0xff0000);
	DrawLineAA(1030.f, 165.f, 1100.f, 270.f, 0xffffff, 5.0f);
	DrawLineAA(1075.f, 260.f, 1100.f, 270.f, 0xffffff, 5.0f);
	DrawLineAA(1100.f, 270.f, 1105.f, 245.f, 0xffffff, 5.0f);

	//青 (水属性)
	DrawCircleAA(830.f, 350.f, 60.f, 32, 0x3c78d8, TRUE);
	DrawString(770, 312, " Blue\n(Water)", 0xFFFF55);
	DrawString(770, 180, "Damage", 0x0000ff);
	DrawLineAA(870.f, 265.f, 930.f, 165.f, 0xffffff, 5.0f);
	DrawLineAA(907.f, 167.f, 930.f, 165.f, 0xffffff, 5.0f);
	DrawLineAA(930.f, 165.f, 935.f, 190.f, 0xffffff, 5.0f);

	//緑\n(木属性)
	DrawCircleAA(1140.f, 350.f, 60.f, 32, 0x6aa84f, TRUE);
	DrawString(1080, 312, " Green\n(Wood)", 0xFFFF55);
	DrawString(935, 390, "Damage", 0x00ff00);
	DrawLineAA(906.f, 344.f, 1060.f, 344.f, 0xffffff, 5.0f);
	DrawLineAA(906.f, 344.f, 932.f, 320.f, 0xffffff, 5.0f);
	DrawLineAA(906.f, 344.f, 932.f, 366.f, 0xffffff, 5.0f);

	DrawBoxAA(960, 603, 1242, 702, 0xffffff, FALSE, 2.0f);
	DrawBoxAA(962, 605, 1240, 700, 0x555555, TRUE, 2.0f);
	DrawString(980, 630, "B Button:Back", 0xffffff);
}

void GameMain::UpdateGameClear()
{
	//クレジットを見てほしいのでカーソルを動かせないように
	cursor = 1;

	cursorOld = cursor;
	if (set_sound_once == false)
	{
		ResourceManager::StopSound(bgm_normal);
		ResourceManager::StopSound(bgm_noise);
		ResourceManager::StopSound(bgm_abnormal);
		ResourceManager::StartSound(game_clear_bgm, FALSE);
		back_ground->SetIsClear(true);
		set_sound_once = true;
	}

	if (clear_timer++ > 250) {

		ResourceManager::StartSound(decision_se);
		now_scene = new End();

	}
	if (clear_timer < 250 && clear_timer > 150) {
		blackout -= 3;
	}
}

void GameMain::DrawGameClear()const
{
	if (clear_timer <= 90) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (clear_timer * 2));
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	if (clear_timer > 90) {

		int fontsize = 192 / 2;
		SetFontSize(fontsize * 2);
		DrawString(200, 100, "G", 0xff0000);
		DrawString(200 + fontsize * 1, 100, "A", 0xffffff);
		DrawString(200 + fontsize * 2, 100, "M", 0xffffff);
		DrawString(200 + fontsize * 3, 100, "E", 0xffffff);
		DrawString(200 + fontsize * 4, 100, "C", 0x00ff00);
		DrawString(200 + fontsize * 5, 100, "L", 0xffffff);
		DrawString(200 + fontsize * 6, 100, "E", 0xffffff);
		DrawString(200 + fontsize * 7, 100, "A", 0xffffff);
		DrawString(200 + fontsize * 8, 100, "R", 0x0000ff);

		SetFontSize(48);
	}
	SetDrawBright(blackout, blackout, blackout);
	
	if (clear_timer > 250) {
		SetDrawBright(255, 255, 255);
	}
}

void GameMain::UpdateGameOver()
{

	cursorOld = cursor;
	if (set_sound_once == false)
	{
		ResourceManager::StopSound(bgm_normal);
		ResourceManager::StopSound(bgm_noise);
		ResourceManager::StopSound(bgm_abnormal);
		ResourceManager::StartSound(bgm_title, TRUE);
		set_sound_once = true;
	}

	if (PadInput::TipLeftLStick(STICKL_X) < -0.5f || PadInput::OnButton(XINPUT_BUTTON_DPAD_LEFT))
	{
		cursor = 0;
		if (cursorOld == 1) {
			ResourceManager::StartSound(cursor_se);
		}
	}
	else if (PadInput::TipLeftLStick(STICKL_X) > 0.5f || PadInput::OnButton(XINPUT_BUTTON_DPAD_RIGHT))
	{
		cursor = 1;
		if (cursorOld == 0) {
			ResourceManager::StartSound(cursor_se);
		}
	}

	if (PadInput::OnButton(XINPUT_BUTTON_B)) {
		switch (cursor)
		{
		case 0:
			ResourceManager::StartSound(decision_se);
			set_sound_once = false;
			SetStage(now_stage, true);
			gm_state = GameMainState::S_GameMain;
			pause_after_flg = true;
			ResourceManager::StopSound(bgm_title);
			//BGMの再生
			if (now_stage == 0)
			{
				ResourceManager::StartSound(bgm_normal, TRUE);
				ResourceManager::StartSound(bgm_noise, TRUE);
			}
			break;
		case 1:
			cursor = 0;
			before_check_scene = gm_state;
			gm_state = GameMainState::Check;
			break;
		default:
			break;
		}
	}


	if (circleAng++ >= 360.f) {
		circleAng = 0.f;
	}
}

void GameMain::DrawGameOver()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawBoxAA(200, 410, 500, 510, 0x000000, TRUE);
	DrawBoxAA(200, 410, 500, 510, 0xffffff, FALSE);
	DrawBoxAA(780, 410, 1080, 510, 0x000000, TRUE);
	DrawBoxAA(780, 410, 1080, 510, 0xffffff, FALSE);

	int fontsize = 192 / 2;
	SetFontSize(fontsize * 2);
	DrawString(240, 100, "G", 0xff0000);
	DrawString(240 + fontsize * 1, 100, "A", 0xffffff);
	DrawString(240 + fontsize * 2, 100, "M", 0xffffff);
	DrawString(240 + fontsize * 3, 100, "E", 0xffffff);
	DrawString(240 + fontsize * 4, 100, "O", 0x00ff00);
	DrawString(240 + fontsize * 5, 100, "V", 0xffffff);
	DrawString(240 + fontsize * 6, 100, "E", 0xffffff);
	DrawString(240 + fontsize * 7, 100, "R", 0x0000ff);
	SetFontSize(48);
	DrawString(260, 436, "RESTART", 0xffffff);
	DrawString(860, 436, "TITLE", 0xffffff);

	Vector2D circleLoc;

	if (cursor == 0) {
		circleLoc.x = 350.f;
		circleLoc.y = 460.f;
	}
	else {
		circleLoc.x = 930.f;
		circleLoc.y = 460.f;
	}

	DrawCircleAA(circleLoc.x, circleLoc.y, 40.f * 2.3f, 40, 0xffff00, FALSE, 4.f * 2.3f);

	Vector2D base;
	base.x = circleLoc.x;
	base.y = circleLoc.y;

	Vector2D l[3];
	l[0].x = base.x;
	l[0].y = base.y - 40.f * 2.3f;

	l[0] = RotationLocation(base, l[0], (float)(circleAng * M_PI / 180));

	l[1] = RotationLocation(base, l[0], (float)(120.f * M_PI / 180));

	l[2] = RotationLocation(base, l[0], (float)(240.f * M_PI / 180));


	DrawCircleAA(l[0].x, l[0].y, 15.f * 1.5f, 32, 0xcc0000, TRUE);
	DrawCircleAA(l[1].x, l[1].y, 15.f * 1.5f, 32, 0x3c78d8, TRUE);
	DrawCircleAA(l[2].x, l[2].y, 15.f * 1.5f, 32, 0x6aa84f, TRUE);
}

void GameMain::UpdateCheck()
{

	cursorOld = cursor;

	if (PadInput::TipLeftLStick(STICKL_X) < -0.5f || PadInput::OnButton(XINPUT_BUTTON_DPAD_LEFT))
	{
		cursor = 0;
		if (cursorOld == 1) {
			ResourceManager::StartSound(cursor_se);
		}
	}
	else if (PadInput::TipLeftLStick(STICKL_X) > 0.5f || PadInput::OnButton(XINPUT_BUTTON_DPAD_RIGHT))
	{
		cursor = 1;
		if (cursorOld == 0) {
			ResourceManager::StartSound(cursor_se);
		}
	}

	if (PadInput::OnButton(XINPUT_BUTTON_B)) {
		switch (cursor)
		{
		case 0:
			ResourceManager::StartSound(decision_se);
			gm_state = before_check_scene;
			break;
		case 1:
			ResourceManager::StartSound(decision_se);
			now_scene = new Title();
			break;
		default:
			break;
		}
	}


	if (circleAng++ >= 360.f) {
		circleAng = 0.f;
	}
}

void GameMain::DrawCheck()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawString((SCREEN_WIDTH / 2) - 150, (SCREEN_HEIGHT / 2) - 240, "Are you sure?", 0xffffff);

	DrawBoxAA(200, 410, 500, 510, 0x000000, TRUE);
	DrawBoxAA(200, 410, 500, 510, 0xffffff, FALSE);
	DrawBoxAA(780, 410, 1080, 510, 0x000000, TRUE);
	DrawBoxAA(780, 410, 1080, 510, 0xffffff, FALSE);

	SetFontSize(48);
	DrawString(300, 436, "NO", 0xffffff);
	DrawString(870, 436, "YES", 0xffffff);

	Vector2D circleLoc;

	if (cursor == 0) {
		circleLoc.x = 350.f;
		circleLoc.y = 460.f;
	}
	else {
		circleLoc.x = 930.f;
		circleLoc.y = 460.f;
	}

	DrawCircleAA(circleLoc.x, circleLoc.y, 40.f * 2.3f, 40, 0xffff00, FALSE, 4.f * 2.3f);

	Vector2D base;
	base.x = circleLoc.x;
	base.y = circleLoc.y;

	Vector2D l[3];
	l[0].x = base.x;
	l[0].y = base.y - 40.f * 2.3f;

	l[0] = RotationLocation(base, l[0], (float)(circleAng * M_PI / 180));

	l[1] = RotationLocation(base, l[0], (float)(120.f * M_PI / 180));

	l[2] = RotationLocation(base, l[0], (float)(240.f * M_PI / 180));


	DrawCircleAA(l[0].x, l[0].y, 15.f * 1.5f, 32, 0xcc0000, TRUE);
	DrawCircleAA(l[1].x, l[1].y, 15.f * 1.5f, 32, 0x3c78d8, TRUE);
	DrawCircleAA(l[2].x, l[2].y, 15.f * 1.5f, 32, 0x6aa84f, TRUE);

}

void GameMain::UpdateState(GameMainState _state)
{
	gm_state = _state;
}

void GameMain::SetStageBlockAround()
{
	for (int i = 0; i < stage_height_num; i++)
	{
		for (int j = 0; j < stage_width_num; j++)
		{
			//ブロックだけダウンキャストする
			if (object[stage_block_pos[i][j]] != nullptr && object[stage_block_pos[i][j]]->GetObjectType() == BLOCK)
			{
				for (int k = 0; k < 8; k++)
				{
					static_cast<Stage*>(object[stage_block_pos[i][j]])->SetAroundBlock(k, CheckAroundBlock(i, j, k));
				}
			}
		}
	}
}

int GameMain::CheckAroundBlock(int _i, int _j, int _num)
{
	//引数_iが0（上端）で、更に上のデータを参照しようとした場合、-1を返す
	if (_i == 0 && (_num == 0 || _num == 1 || _num == 2))
	{
		return -1;
	}
	//引数_iがstage_height_num - 1（下端）で、更に下のデータを参照しようとした場合、-1を返す
	if (_i == stage_height_num - 1 && (_num == 5 || _num == 6 || _num == 7))
	{
		return -1;
	}
	//引数_jが0（左端）で、更に左のデータを参照しようとした場合、-1を返す
	if (_i == 0 && (_num == 0 || _num == 3 || _num == 5))
	{
		return -1;
	}
	//引数_jがstage_width_num - 1（右端）で、更に右のデータを参照しようとした場合、-1を返す
	if (_i == stage_width_num - 1 && (_num == 2 || _num == 4 || _num == 7))
	{
		return -1;
	}
	//指定した位置のステージデータを返す
	switch (_num)
	{
	case 0:
		return stage_data[_i - 1][_j - 1];
	case 1:
		return stage_data[_i - 1][_j];
	case 2:
		return stage_data[_i - 1][_j + 1];
	case 3:
		return stage_data[_i][_j - 1];
	case 4:
		return stage_data[_i][_j + 1];
	case 5:
		return stage_data[_i + 1][_j - 1];
	case 6:
		return stage_data[_i + 1][_j];
	case 7:
		return stage_data[_i + 1][_j + 1];
	}
	return -1;
}

void GameMain::InScreenUpdate()
{
	for (int i = 0; object[i] != nullptr; i++)
	{
		//既にスクリーン内に入っているオブジェクトなら、追加処理は省略する
		auto result = std::find(in_screen_object.begin(), in_screen_object.end(), object[i]);
		if (result == in_screen_object.end())
		{
			//スクリーン内に入っているオブジェクトを追加する
			if (CheckInScreen(object[i]))
			{
				in_screen_object.push_back(object[i]);
			}
		}
	}
	//画面外に出たオブジェクトを配列から除外する
	for (auto it =in_screen_object.begin();it != in_screen_object.end();)
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
