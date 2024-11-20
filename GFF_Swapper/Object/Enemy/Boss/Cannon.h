#pragma once
#include "../../Base/Object.h"
#include "../../../Utility/common.h"
#include "BossAttackFire.h"
#include "BossAttackWood.h"
#include "BossAttackWater.h"
#include "../../../Scene/Camera.h"

#define CANNON_STOP 100  //プレイヤーがこの数値より近いところにいるなら、弾を止める
#define EAST  0  //東
#define WEST  1  //西
#define SOUTH 2  //南
#define NORTH 3  //北

class Cannon :
    public Object
{
private:
	class Camera* camera;	//カメラ
    int cannon_type;        //打ち出す弾の種類 0=赤 1=緑 2=青
    int cannon_angle;       //打ち出す弾の向き 4方向　東西南北
	bool cannon_stop;		//弾を撃つか判断
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_type">打ち出す弾の種類 0=赤 1=緑 2=青</param>
	/// <param name="_angle">打ち出す弾の向き 4方向</param>
	Cannon(int _type, int _angle);
	~Cannon();

	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Finalize()override;

	void Update(ObjectManager* _manager)override;
	void Draw()const override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override;
};

