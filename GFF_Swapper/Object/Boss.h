#pragma once
#include "Object.h"
#include <vector>
#include <array>
#include <ctime>
#include <mutex> 
#include "EnemyBat.h"
#include "BossAttackFire.h"
#include "BossAttackWater.h"
#include "BossAttackWood.h"

enum class BossState {
	MOVE = 0,
	ATTACK,
	DEATH
};

class Boss :
	public Object
{
private:
	enum hitPosition
	{
		top,
		bottom,
		left,
		right
	};
	Location vector;		//ボスのベクトル
	bool stageHitFlg[2][4]; //bossとステージの当たり判定
	float move[4];			//各方向加速度格納用

	BossState boss_state;// 現在のボスの状態

	float barrier_rad[3];	//バリアの半径
	int barrier_num;		//バリアの数

	int wing_color;
	int part_color[3];

	bool damage_flg;				//ダメージを受けたとき
	int damage_effect_time = 60;	//ダメージエフェクトの持続時間
	bool damage_effect_flg = false; // ダメージエフェクトのフラグ

	bool damage_anim_flg;
	int damage_anim_time;
	int shake_anim;

	float speed;				//移動速度;

	int boss_color;
	//計算用
	int cunt;
	int c;
	int num;
	int t = 0;

	int state_change_time;		//状態変更のタイミング

	std::vector<Location> vertices; //ボスの頂点情報
	std::vector<Location> warp_pos; //ワープ位置情報
	std::array<Location, 40> wing;	//ボスの羽の位置情報
	std::array<Location, 40> wing_mirror;//ミラー羽の位置情報

	int wing_fps; //羽の加算用
	float boss_anim;//アニメーション用

	bool f = false;
	bool oldF = false;
	int cnt = 100;
	int attack = 0;
	int old_attack = 0;
	int attack_num = 0;
	bool side = false;

	float attackWood[6];
	int woodNum = 0;

	int damage_se;	//被ダメージSE

public:
	Boss();
	~Boss();

	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

	//ボスの移動処理
	void Move(GameMain* _g);

	//ヒット時の処理
	void Hit(Object* _object)override;

	bool SearchColor(Object* ob) {
		return 0;
	}
	//ステージとの当たり判定
	bool CheckCollision(Location l, Erea e);

	//プレイヤーとボスの距離計算
	float DistanceCalc(Location pos1, Location pos2);

	//ボスのバリア
	void barrier();

	//ボスの攻撃
	void BossAtack(GameMain *_g);

	//六角形模様球体の描画
	void DrawHexagonSphere() const;

	//六角形描画
	void DrawHexagon(Location center, float size, int color) const;

	//羽を描画する
	void DrawWings() const;

	//羽の位置を更新する
	void UpdateWingPositions();

	//羽の位置を設定する
	void InvertedWingPositions();

	//羽の座標をファイルに保存
	void SavePosition();

	//羽の座標をファイルから読み込む
	void LoadPosition();

	//状態を渡す
	BossState GetBossState(){
		return boss_state;
	}
};