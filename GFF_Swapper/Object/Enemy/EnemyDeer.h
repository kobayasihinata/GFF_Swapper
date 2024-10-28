#pragma once
#include "../Base/Object.h"
#include "../Base/CharaBase.h"
#include "../../Utility/common.h"

# define PI 3.14159265359

using namespace std;

enum class DeerState {
	GRAVITY = 0,
	IDLE,
	LEFT,
	RIGHT,
	DEATH
};

enum class d_draw
{
	vr_one,
	vr_two
};

class EnemyDeer :
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

	DeerState deer_state;
	d_draw deer_draw;

	bool deer_spawn;
	int deer_death_timer = 0;

	float tn = 0.0f;

	float move[4]; //各方向加速度格納用
	bool stageHitFlg[2][4]; //プレイヤーステージの当たり判定
	//0の行はステージのブロック一つ一つに対しての判定
	//1の行は1フレームでステージ全体との判定

	float r_x = 0.0f, r_y = 0.0f;
	float d_rad = 0.0f;
	float d_leg_rad[4];
	float d_neck_rad = 0.0f;
	float d_head_rad = 0.0f;
	float d_eye_rad = 0.0f;
	float d_left_leg[4];
	bool left_Anim_Flg[4];

	int walk_se;		//歩行効果音
	int damage_se[3];	//被ダメージSE格納

	int anim_fps;
	float leg_angle[4];
	bool anim_flg[4];
	float speed;

public:

	EnemyDeer();
	~EnemyDeer();
	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override {
		return false;
	}

	void EnemyDeerMove(void);	 //移動
	bool CheckCollision(Vector2D l, Vector2D e); //ステージとの当たり判定

	void DeerAnim();
};

