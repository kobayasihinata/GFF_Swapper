#pragma once
#include "../../Base/Object.h"
#include "../../../Utility/common.h"

class BossAttackWood : public Object
{
private:
	class Camera* camera;                                      //カメラ座標
	float start_location_y;	//スポーン位置の高さ
	Vector2D velocity;
	float bambooHeight;  //高さ
	Vector2D startLoc;

	int spawn_count;	//生え始めるまでの時間
	int delete_count;	//生え終わってからの経過時間
	int w_type;	//もとから生えてるのか地面から生えてくるのか
	int knot;   //竹の節の数
	bool camera_impact_once;	//カメラ振動
	int start_se;		//攻撃開始SE
public:
	BossAttackWood();
	~BossAttackWood();

	void Initialize(Vector2D _location, Vector2D _erea, int _color_data)override;
	void Finalize()override;

	void Update(ObjectManager* _manager)override;
	void Draw()const override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override;

	void MoveBamboo();
};