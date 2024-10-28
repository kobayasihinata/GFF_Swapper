#pragma once
#include <vector>
#include "../../Utility/common.h"
#include "../Base/Object.h"
#include "../../Scene/GameMain.h"

enum class BatState {
	IDLE = 0,
	LEFT,
	RIGHT,
	DEATH
};

class EnemyBat:
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
	Vector2D vector; //コウモリのベクトル
	BatState bat_state;	//コウモリの状態
	//Object* delete_object;//消すオブジェクト
	bool stageHitFlg[2][4]; //コウモリとステージの当たり判定
	float move[4];//各方向加速度格納用

	float wing_angle;	//羽の動かす用
	int up;		//sin波で移動する用

	//bool hit_flg[3];

	int death_timer;	//死亡演出

	int wing_se;		//羽SE
	int damage_se[3];	//被ダメージSE格納

	bool se_once;		//一回だけ再生	

public:
	EnemyBat();
	~EnemyBat();
	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

public:
	void Move(GameMain* _g);

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) {
		return 0;
	}

	bool CheckCollision(Vector2D l, Vector2D e); //ステージとの当たり判定
};

