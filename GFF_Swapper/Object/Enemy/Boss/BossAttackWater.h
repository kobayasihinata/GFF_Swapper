#pragma once
#include "../../Base/Object.h"
class BossAttackWater : public Object
{
private:
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

	int start_se;	//攻撃開始SE
	int end_se;		//攻撃終了SE

public:
	BossAttackWater();
	~BossAttackWater();

	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Finalize()override;

	void Update(GameMain* _g)override;
	void Draw()const override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override;

};