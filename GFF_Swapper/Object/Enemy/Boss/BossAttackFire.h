#pragma once
#include "../../Base/Object.h"
class BossAttackFire : public Object
{
private:
	class Camera* camera;                                      //カメラ座標
	Vector2D velocity;
	bool flg;
	bool hitFlg;
	int boundCnt;

	int start_se;	//攻撃開始SE
	int end_se;		//攻撃終了SE

public:
	BossAttackFire();
	~BossAttackFire();

	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Finalize()override;

	void Update(ObjectManager* _manager)override;
	void Draw()const override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override;

};