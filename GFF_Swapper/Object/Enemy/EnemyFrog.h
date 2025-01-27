#pragma once
#include "../Base/Object.h"

#define JUMP_COOLDOWN 40	//蛙の次のジャンプまでの時間

enum class FrogState {
	RIGHT_IDOL=0,
	LEFT_IDOL,
	LEFT_JUMP,
	RIGHT_JUMP,
	FAINT,
	DEATH
};

class EnemyFrog :
    public Object
{
public:
	enum hitPosition
	{
		top,
		bottom,
		left,
		right
	};
private:
	class Camera* camera;				 //カメラ情報
	int timer;				 //時間測定
	FrogState old_state;	 //一つ前のカエルの状態
	FrogState frog_state;	 //カエルの状態

	bool death_flg;			 //生きているか
	Vector2D velocity;		 //カエルのベクトル
	bool stageHitFlg[2][4];  //カエルとステージの当たり判定
	float move[4];			 //各方向加速度格納用
	Vector2D old_location;	 //ジャンプ前の座標格納

	float faint_timer;		//気絶している時間測定

	int jump_cooldown_timer; //次のジャンプまでの時間測定
	int jump_timer;			 //ジャンプしている間の時間測定
	float face_angle;		 //顔の向き
	int death_timer;		 //死亡演出
	bool effect_once;		 //エフェクトを一回だけ出す用

	int frog_image[4];		 //カエル画像格納
	int jump_image[4];		 //跳躍カエル画像格納
	int damage_image[4];		 //跳躍カエル画像格納
	int jump_se;			 //ジャンプSE
	int damage_se[3];		 //被ダメージSE格納
	int faint_se;			//スタンSE
	int fall_se;			//落下SE
	int hit_se;				//ぶつかるSE
public:
	EnemyFrog();
	~EnemyFrog();
	void Initialize(Vector2D _location, Vector2D _erea, int _color_data)override;
	void Update(ObjectManager* _manager)override;
	void Draw()const override;
	void Finalize()override;
	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override {
		return false;
	}

private:
	void JumpFrogDraw(Vector2D location,float _angle)const;		//ジャンプ中カエルの描画
	void IdolFrogDraw(Vector2D location,bool _direction)const;		//通常カエルの描画
	void Move(ObjectManager* _manager);		//移動系処理
	void UpdataState(ObjectManager* _manager);	//カエルの状態更新
	bool CheckCollision(Vector2D l, Vector2D e); //ステージとの当たり判定
	void SetFrogState(FrogState _state);			//カエルの状態変更
};

