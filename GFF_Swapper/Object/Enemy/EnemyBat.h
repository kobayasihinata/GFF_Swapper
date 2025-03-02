#pragma once
#include <vector>
#include "../../Utility/common.h"
#include "../Base/Object.h"
#include "../../Scene/GameMain.h"

enum class BatState {
	IDLE = 0,
	LEFT,
	RIGHT,
	FAINT,
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
	class Camera* camera;				 //カメラ情報
	Vector2D vector; //コウモリのベクトル
	BatState bat_state;	//コウモリの状態
	//Object* delete_object;//消すオブジェクト
	bool stageHitFlg[2][4]; //コウモリとステージの当たり判定
	float move[4];//各方向加速度格納用

	float wing_angle;	//羽の動かす用
	int up;		//sin波で移動する用

	//bool hit_flg[3];

	float faint_timer;	//スタン時間測定
	int death_timer;	//死亡演出

	int bat_image;		//コウモリ画像格納
	int damage_image;	//コウモリダメージ画像格納
	int wing_se;		//羽SE
	int damage_se[3];	//被ダメージSE格納
	int faint_se;			//スタンSE
	int fall_se;			//落下SE
	int hit_se;				//ぶつかるSE

	float len = 0.0f;
	bool wall_hit_flg = false;
	int wall_hit_timer = 0;

	bool se_once;		//一回だけ再生	

	int bat_state_num;

public:
	EnemyBat();
	~EnemyBat();
	void Initialize(Vector2D _location, Vector2D _erea, int _color_data)override;
	void Update(ObjectManager* _manager)override;
	void Draw()const override;
	void Finalize()override;

public:
	void Move(ObjectManager* _manager);

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) {
		return 0;
	}

	bool CheckCollision(Vector2D l, Vector2D e); //ステージとの当たり判定
};

