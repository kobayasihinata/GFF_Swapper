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

//木の攻撃がスポーンする位置
static float WOOD_SPAWN[6][3]{
	{570,610,650},
	{315,355,395},
	{825,865,905},
	{80,120,160},
	{1080,1120,1160},
	{-825,-865,-905},
};

//ボスの状態
enum class BossState {
	MOVE = 0,
	ATTACK,
	DEATH
};

class Boss :
	public Object
{
private:
	//ボス移動関連変数
	
	//当たり判定用
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

	//ボス情報関連変数
	BossState boss_state;			//現在のボスの状態
	float barrier_rad[3];			//バリアの半径
	int barrier_num;				//バリアの数
	int wood_count;					//木攻撃の数カウント
	int fire_count;					//火攻撃の数カウント
	int attack_count;				//何回攻撃したか測定
	bool stop_flg;					//ボスの動作を止めておくか判断
	bool invin_flg;					//無敵かどうか
	int state_change_time;			//状態変更のタイミング
	int boss_appeared_timer;		//登場演出の時間測定

	//ボスダメージ、死亡関連変数
	bool damage_flg;				//ダメージを受けたとき
	int damage_effect_time;			//ダメージエフェクトの持続時間
	bool damage_effect_flg;			//ダメージエフェクトのフラグ
	bool damage_anim_flg;			//ダメージアニメーション用フラグ
	int damage_anim_time;			//ダメージアニメーション持続時間
	int shake_anim;					//ダメージ時の振動数値管理用
	bool death_flg;					//死亡演出中か
	int death_timer;				//死亡演出の時間測定

	//ボス色関連変数
	float change_color_timer;		//色変化にかける時間
	int next_color;					//次に変わる色
	int change_rand;				//ボスを揺らす
	int boss_color;					//ボスの描画色

	//計算用
	Vector2D velocity;						//加速度
	Vector2D player_local_location;			//プレイヤーの座標
	int cunt;								//羽の位置計算用
	int tutirial_num;						//羽の位置計算用
	int attack_timer;						//攻撃の時間測定用
	std::vector<Vector2D> vertices;			//ボスの頂点情報
	std::vector<Vector2D> warp_pos;			//ワープ位置情報
	std::array<Vector2D, 40> wing;			//ボスの羽の位置情報
	std::array<Vector2D, 40> wing_mirror;	//ミラー羽の位置情報
	float boss_anim;						//アニメーション用
	bool attack_flg;						//攻撃しているか判断
	bool old_attack_flg;					//１フレーム前に攻撃していたか判断
	int cnt;								//攻撃の時間測定
	int attack_type;						//攻撃の種類格納 0=火 1=木 2=水	
	int old_attack;							//ひとつ前の攻撃を格納
	int attack_num;							//攻撃した回数を格納
	bool side;								//画面の右側に居るか左側に居るか false = 右側


	//SE格納用変数
	int warphole_se;	//ワープホールSE
	int spawn_se;		//登場SE
	int damage_se;		//被ダメージSE
	int appeared_se;	//登場SE
	int bgm_abnormal;	//ボス曲格納
	int death_se;		//死亡SE
	int death_expro_se;	//死亡爆破SE

public:
	Boss();
	~Boss();

	void Initialize(Vector2D _location, Vector2D _erea, int _color_data)override;
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
	void BossAttack(ObjectManager* _manager);

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
	BossState GetBossState()const{
		return boss_state;
	}
};