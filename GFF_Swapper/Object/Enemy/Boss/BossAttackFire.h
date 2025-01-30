#pragma once
#include "../../Base/Object.h"
#include "../../../Utility/common.h"
class BossAttackFire : public Object
{
private:
	class Camera* camera;                                      //カメラ座標
	Vector2D parent_center_location;						   //このオブジェクトを生成したオブジェクトの中心座標
	Vector2D velocity;
	bool flg;
	bool hitFlg;
	int boundCnt;
	bool player_hit;		//プレイヤーに当たったか判断
	int angle;		//弾の向き 0~3の東西南北
	int start_se;	//攻撃開始SE
	int end_se;		//攻撃終了SE

public:
	BossAttackFire(Vector2D _parent_center_location, int _angle = -1);
	~BossAttackFire();

	void Initialize(Vector2D _location, Vector2D _erea, int _color_data)override;
	void Finalize()override;

	void Update(ObjectManager* _manager)override;
	void Draw()const override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override;

	void SetAngle(ObjectManager* _manager);
};