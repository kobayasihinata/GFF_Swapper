#pragma once
#include "../../Base/Object.h"
#include "../../../Utility/common.h"
class BossAttackWood : public Object
{
private:
	class Camera* camera;                                      //カメラ座標
	Vector2D velocity;
	float bambooHeight;  //高さ
	Vector2D startLoc;

	int count;	//生え始めるまでのカウント
	int w_type;	//もとから生えてるのか地面から生えてくるのか
	int knot;   //竹の節の数
	bool camera_impact_once;	//カメラ振動
	bool player_hit;		//プレイヤーに当たったか
	int start_se;		//攻撃開始SE
public:
	BossAttackWood();
	~BossAttackWood();

	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Finalize()override;

	void Update(ObjectManager* _manager)override;
	void Draw()const override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override;

	void MoveBamboo();
};