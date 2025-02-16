#pragma once
#include "../../Base/Object.h"
#include "../../../Utility/common.h"
#include "BossAttackFire.h"
#include "BossAttackWood.h"
#include "BossAttackWater.h"
#include "../../../Scene/Camera.h"

#define CANNON_STOP 100  //プレイヤーがこの数値より近いところにいるなら、弾を止める

//弾の発射間隔(フレーム)赤緑青の順番
static int cannon_cooldown[3]
{
	120,
	300,
	90
};

class Cannon :
    public Object
{
private:
	class Camera* camera;	//カメラ
    int cannon_type;        //打ち出す弾の種類 0=赤 1=緑 2=青
    int cannon_angle;       //打ち出す弾の向き 4方向　東西南北
	bool cannon_stop;		//弾を撃つか判断

	int cannon_image;		//砲台の画像
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_type">打ち出す弾の種類 0=赤 1=緑 2=青</param>
	/// <param name="_angle">打ち出す弾の向き 4方向</param>
	Cannon(int _type, int _angle);

	//デストラクタ
	~Cannon();

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

