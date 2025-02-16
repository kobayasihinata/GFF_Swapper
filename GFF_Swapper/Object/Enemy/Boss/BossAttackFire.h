#pragma once
#include "../../Base/Object.h"
#include "../../../Utility/common.h"
class BossAttackFire : public Object
{
private:
	class Camera* camera;				//カメラ座標
	Vector2D parent_center_location;	//このオブジェクトを生成したオブジェクトの中心座標
	Vector2D velocity;					//移動量
	bool set_velocity_once;				//移動量の決定用			
	bool hit_flg;						//ボス火攻撃が消えるか判断
	int bound_cnt;						//壁面に反映した数を測定
	bool player_hit;					//プレイヤーに当たったか判断
	int angle;							//弾の向き 0~3の東西南北

	int start_se;						//攻撃開始SE
	int end_se;							//攻撃終了SE

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_parent_center_location">プレイヤーの中心座標</param>
	/// <param name="_angle">角度　砲台からの発射時は数字を入れる</param>
	BossAttackFire(Vector2D _parent_center_location, int _angle = -1);
	
	//デストラクタ
	~BossAttackFire();

	void Initialize(Vector2D _location, Vector2D _erea, int _color_data)override;
	void Finalize()override;

	void Update(ObjectManager* _manager)override;
	void Draw()const override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override;

	void SetAngle(ObjectManager* _manager);
};