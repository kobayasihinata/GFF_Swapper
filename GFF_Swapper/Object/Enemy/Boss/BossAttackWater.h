#pragma once
#include "../../Base/Object.h"
#include "../../../Utility/common.h"
class BossAttackWater : public Object
{
private:
	class Camera* camera;                                      //カメラ座標

	Vector2D velocity;
	Vector2D unitVelocity;
	bool flg;
	bool moveFlg;
	bool hitFlg;
	int count;
	float len;
	Vector2D f_location;
	Vector2D f_erea;
	float rad;
	bool player_hit;	//プレイヤーに当たったか

	int start_se;	//攻撃開始SE
	int end_se;		//攻撃終了SE

public:
	BossAttackWater();
	~BossAttackWater();

	void Initialize(Vector2D _location, Vector2D _erea, int _color_data)override;
	void Finalize()override;

	void Update(ObjectManager* _manager)override;
	void Draw()const override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override;

};