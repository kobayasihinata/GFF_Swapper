#pragma once
#include "../../Base/Object.h"
#include "../../../Utility/common.h"

class BossAttackWood : public Object
{
private:
	class Camera* camera;       //カメラ座標
	float start_location_y;		//スポーン位置の高さ
	Vector2D velocity;			//移動量
	Vector2D start_loc;			//初期座標保存

	int spawn_count;			//生え始めるまでの時間
	int delete_count;			//生え終わってからの経過時間
	int w_type;					//もとから生えてるのか地面から生えてくるのか
	bool camera_impact_once;	//カメラ振動
	int start_se;				//攻撃開始SE
public:

	//コンストラクタ
	BossAttackWood();

	//デストラクタ
	~BossAttackWood();

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

	//竹移動処理
	void MoveBamboo();
};