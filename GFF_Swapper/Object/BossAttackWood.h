#pragma once
#include "Object.h"
class BossAttackWood : public Object
{
private:
	Location velocity;
	float bambooHeight;  //高さ
	Location startLoc;

	int count;	//生え始めるまでのカウント
	int w_type;	//もとから生えてるのか地面から生えてくるのか
	int knot;   //竹の節の数
	bool camera_impact_once;	//カメラ振動

	int start_se;		//攻撃開始SE
public:
	BossAttackWood();
	~BossAttackWood();

	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Finalize()override;

	void Update(GameMain* _g)override;
	void Draw()const override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override;

	void MoveBamboo();
};