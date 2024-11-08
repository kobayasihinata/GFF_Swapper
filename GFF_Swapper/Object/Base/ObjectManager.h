#pragma once
#include "DxLib.h"
#include "Object.h"
#include "../../Scene/Camera.h"
#include "../../Object/EffectSpawner.h"

#include"../../Scene/eNum/GameMainState.h"

#include <vector>
#include <map>

class GameMain;

//オブジェクト生成時に必要な情報
struct SpawnData
{
	Object* object;
	Vector2D location;
	Vector2D size;
	int color;
	int stage_around_data[8];
};

class ObjectManager
{
private:
	int frame;									//フレーム計測
	class Camera* camera;						//カメラ取得
	std::vector<Object*> object_list;			//プレイヤーとボス以外のすべてのオブジェクト格納
	Object* player_object;						//プレイヤーオブジェクト格納
	Object* boss_object;						//プレイヤーオブジェクト格納
	std::vector<Object*> in_screen_object;      //画面内にあるオブジェクト格納
	std::vector<SpawnData> create_object;		//生成するオブジェクト格納
	std::vector<Object*> delete_object;			//削除するオブジェクト格納
	Object* now_current_object;                 //現在交換対象になっているオブジェクト
	EffectSpawner* effect_spawner;				//エフェクト管理オブジェクト
	int object_num;								//オブジェクト数
	int move_object_num;						//Updateされているオブジェクト数
	bool boss_blind_flg;						//ボスステージの暗転が有効か
	GameMainState change_state;					//次に変えるゲームメインのステート

public:
	bool player_respawn_flg;					//プレイヤーをリスポーンさせるか判断
	Vector2D player_respawn;					//プレイヤーが生成された地点を保存しておく
	int change_stage;							//次に変えるゲームメインのステージ

public:
	void Initialize();				//初期化処理
	void Update(GameMain* _g);		//オブジェクト更新処理
	void Draw()const;				//描画処理
	void Finalize();				//終了時処理

	void CreateObject(Object* _object, Vector2D _location, Vector2D _erea, int _color_data);	//オブジェクトの生成
	void CreateObject(SpawnData _data);	//オブジェクトの生成
	void CreatePlayer(Object* _object, Vector2D _location, Vector2D _erea, int _color_data);	//プレイヤーの生成
	void CreateBoss(Object* _object, Vector2D _location, Vector2D _erea, int _color_data);		//ボスの生成
	void DeleteObject(Object* _object);					//オブジェクトの削除
	void DeleteAllObject(bool _player_delete);			//全てのオブジェクトの削除
	void HitCheck();									//当たり判定の確認
	void InScreenUpdate();								//画面内オブジェクトの確認
	bool CheckInScreen(Object* _object)const;			//そのオブジェクトが画面内に居るか判断

	void PlayerUpdate(GameMain* _g);					//プレイヤーの更新
	void BossUpdate();			//ボス＆ボス攻撃の更新
	void SetNowCurrentObject(Object* _object);			//現在交換対象になっているオブジェクトを設定する

	bool GetSearchFlg()const;							//交換中か取得
	Vector2D GetPlayerLocation()const;					//プレイヤー座標取得
	Vector2D GetPlayerLocalLocation()const;				//プレイヤーローカル座標取得
	Vector2D GetPlayerErea()const;						//プレイヤー大きさ取得
	int GetPlayerColor()const;							//プレイヤーの色取得

	Vector2D GetBossLocation()const;					//ボスの位置取得

	void UpdateState(GameMainState _state);				//ゲームメインの状態更新

	void SpawnEffect(Vector2D _location, Vector2D _erea, int _type, int _time, int _color, float _angle = 0.0f);	//エフェクトの生成
	int Swap(Object* _object1, Object* _object2);		//交換アニメーション
};

