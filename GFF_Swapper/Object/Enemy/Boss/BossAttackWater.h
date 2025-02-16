#pragma once
#include "../../Base/Object.h"
#include "../../../Utility/common.h"
class BossAttackWater : public Object
{
private:
	class Camera* camera;       //カメラ座標
	Vector2D velocity;			//移動量
	bool set_velocity_once;		//初期移動方向決定用
	bool move_direction;		//移動方向を上下させる用
	bool hit_flg;				//自身を破壊するかどうか
	Vector2D f_location;		//水の中を回転する小さい円の座標
	Vector2D f_erea;			//水の中を回転する小さい円の大きさ
	float rad;					//進行方向
	bool player_hit;			//プレイヤーに当たったか

	int start_se;				//攻撃開始SE
	int end_se;					//攻撃終了SE

public:

	//コンストラクタ
	BossAttackWater();

	//デストラクタ
	~BossAttackWater();

	/// <summary>
	/// 初期処理
	/// </summary>
	/// <param name="_location">スポーン座標</param>
	/// <param name="_erea">大きさ</param>
	/// <param name="_color_data">色情報</param>
	void Initialize(Vector2D _location, Vector2D _erea, int _color_data)override;

	//終了時処理
	void Finalize()override;

	/// <summary>
	/// 描画以外の更新
	/// </summary>
	/// <param name="_manager">このオブジェクトを管理するマネージャークラスのポインタ</param>
	void Update(ObjectManager* _manager)override;

	//描画の更新
	void Draw()const override;

	/// <summary>
	/// 何かと当たった時の処理
	/// </summary>
	/// <param name="_object">当たった相手のポインタ</param>
	void Hit(Object* _object)override;

	/// <summary>
	/// オブジェクトの色を検索する
	/// </summary>
	/// <param name="ob">対象のオブジェクト</param>
	/// <returns>オブジェクトを選択しているか(プレイヤー以外は偽を返す)</returns>
	bool SearchColor(Object* ob) override;

};