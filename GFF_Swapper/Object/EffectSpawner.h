#pragma once
#include "Object.h"
#define EFFECT_NUM	200		//エフェクトの最大数
#define SWAP_EFFECT_NUM 10		//一回のエフェクト時に発生するパーティクルの数
#define AFTERIMAGE_NUM 10		//交換エフェクトの残像の数

enum EffectList {
	PlayerSpawnEffect = 0,
	ShineEffect,
	DeathEffect,
	LandingEffect,
	ExplosionEffect,
	DamageEffect
};

class GameMain;

struct EffectAnim{
	int timer = 0;					//経過時間
	Vector2D location = { 0,0 };			//座標
	Vector2D local_location = { 0,0 };	//ローカル座標
	Vector2D erea = { 0,0 };					//大きさ
	bool spawn_flg = false;		//エフェクトがスポーンしているかどうか
	int  effect_type = 0;			//エフェクトの種類
	bool touch_ground = false;			//地面に触れたか
	float angle = 0.0f;				//移動方向
	int effect_time = 0;			//消えるまでの時間測定用
	float speed = 0.0f;				//移動速度
	int color = 0;					//色
	Vector2D effect_shift[2] = {0,0};	//輝きエフェクトのずれ格納用
	int shine_timer = 0;			//輝きエフェクトの時間測定用
	float rad = 0.0f;					//輝きエフェクトの回転方向
};

struct SwapAnim
{
	bool move_flg;          //移動するか判断
	Vector2D start;          //オブジェクトの座標（アニメーション開始地点）
	Vector2D goal;          //相手のオブジェクトの座標（アニメーション終了地点）
	Vector2D goal_local;    //相手のオブジェクトのローカル座標（アニメーション終了地点）
	Vector2D erea;               //オブジェクトのサイズ
	Vector2D location[SWAP_EFFECT_NUM];      //交換エフェクト用変数
	Vector2D old_location[SWAP_EFFECT_NUM][AFTERIMAGE_NUM];      //残像表示用
	Vector2D local_location[SWAP_EFFECT_NUM];	//ローカル座標
	float default_rad;						   //直線で移動するときのradを格納
	float move_rad[SWAP_EFFECT_NUM];           //交換エフェクト移動用
	bool update_once[SWAP_EFFECT_NUM];           //交換エフェクト移動の向きを一回だけ変更する用
	int color;             //色情報
	int timer;              //エフェクト時間
	float speed[SWAP_EFFECT_NUM];              //エフェクト速度
};

class EffectSpawner
{
private:
	SwapAnim swap_anim[2];  //交換エフェクト用
	int swap_anim_timer;    //交換エフェクト時間用
	bool change_effect_flg;		//色が変わるエフェクト用
	EffectAnim effect[EFFECT_NUM];
	int frame;		//フレーム測定

	//SE格納
	int swap_se;
	int swap_move_se;

public:
	EffectSpawner();
	~EffectSpawner();
	void Initialize();
	void Update(GameMain* _g);
	void Draw()const;
	void Finalize();

	//エフェクトをスポーンさせる _location=座標 _erea=大きさ _effect_type=エフェクトの種類 _time=消えるまでのフレーム数 _color=エフェクトの色 _angle=進行方向
	void SpawnEffect(Vector2D _location,Vector2D _erea,int _effect_type,int _time,int _color,float _angle = 0);

	//ローカル座標の更新
	void SetScreenPosition(Vector2D _world_to_screen, int _num);

	//エフェクト構造体のリセット
	void ResetEffect(int _num);

	//交換アニメーション
	int Swap(Object* _object1, Object* _object2);

	//エフェクトを構成するパーティクルをスポーンさせる _location=座標 _erea=大きさ _type=パーティクルの種類 _time=消えるまでのフレーム数 _speed=速度 _color=エフェクトの色 _angle=進行方向
	void SpawnParticle(Vector2D _location, Vector2D _erea, int _type, int _time, float _speed, int _color, float _angle);

};

