#pragma once
#include "../../Base/Object.h"
#include <vector>
#include <array>
#include <ctime>
#include <mutex> 
#include "../EnemyBat.h"
#include "../../../Utility/common.h"
#include "../../../Scene/Camera.h"
#include "BossAttackFire.h"
#include "BossAttackWater.h"
#include "BossAttackWood.h"

#define APPEARED_TIME 500			//登場演出の時間
#define BOSS_DEATH_TIME 300			//死亡演出の時間

static float WOOD_SPAWN[6][3]{
	{570,610,650},
	{315,355,395},
	{825,865,905},
	{80,120,160},
	{1080,1120,1160},
	{-825,-865,-905},
};

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
	class Camera* camera;			//カメラ位置
	Vector2D appearance_size;		//羽も含めた、見た目の大きさ
	Vector2D vector;				//ボスのベクトル
	bool stageHitFlg[2][4];			//bossとステージの当たり判定
	float move[4];					//各方向加速度格納用

	BossState boss_state;			//現在のボスの状態

	float barrier_rad[3];			//バリアの半径
	int barrier_num;				//バリアの数

	int wood_count;					//木攻撃の数カウント
	int fire_count;					//火攻撃の数カウント
	int wing_color;
	int part_color[3];

	int attack_count;				//何回攻撃したか測定
	bool stop_flg;					//ボスの動作を止めておくか判断
	bool damage_flg;				//ダメージを受けたとき
	int damage_effect_time = 60;	//ダメージエフェクトの持続時間
	bool damage_effect_flg = false; //ダメージエフェクトのフラグ
	bool death_flg;					//死亡演出中か
	int death_timer;				//死亡演出の時間測定
	bool damage_anim_flg;
	int damage_anim_time;
	int shake_anim;

	float speed;					//移動速度;

	float change_color_timer;		//色変化にかける時間
	int next_color;					//次に変わる色
	int change_rand;				//ボスを揺らす
	int boss_color;

	bool invin_flg;					//無敵かどうか

	//計算用
	Vector2D velocity;
	Vector2D player_local_location;	//プレイヤーの座標
	int cunt;
	int c;
	int tutirial_num;
	int t = 0;

	int state_change_time;		//状態変更のタイミング
	int boss_appeared_timer;	//登場演出の時間測定

	std::vector<Vector2D> vertices; //ボスの頂点情報
	std::vector<Vector2D> warp_pos; //ワープ位置情報
	std::array<Vector2D, 40> wing;	//ボスの羽の位置情報
	std::array<Vector2D, 40> wing_mirror;//ミラー羽の位置情報

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

	int warphole_se;	//ワープホールSE
	int spawn_se;		//登場SE
	int damage_se;	//被ダメージSE
	int appeared_se;	//登場SE
	int bgm_abnormal;	//ボス曲格納
	int death_se;		//死亡SE
	int death_expro_se;	//死亡爆破SE

public:
	Boss();
	~Boss();

	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Update(ObjectManager* _manager)override;
	void Draw()const override;
	void Finalize()override;

	//ボスの移動処理
	void Move();

	//ヒット時の処理
	void Hit(Object* _object)override;

	bool SearchColor(Object* ob) {
		return 0;
	}
	//ステージとの当たり判定
	bool CheckCollision(Vector2D l, Vector2D e);

	//プレイヤーとボスの距離計算
	float DistanceCalc(Vector2D pos1, Vector2D pos2);

	//ボスのバリア
	void barrier();

	//ボスの攻撃
	void BossAtack(ObjectManager* _manager);

	//六角形模様球体の描画
	void DrawHexagonSphere(int _color) const;

	//六角形描画
	void DrawHexagon(Vector2D center, float size, int color) const;

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