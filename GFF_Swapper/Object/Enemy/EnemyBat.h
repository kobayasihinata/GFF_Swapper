#pragma once
#include <vector>
#include "../../Utility/common.h"
#include "../Base/Object.h"
#include "../../Scene/GameMain.h"

enum class BatState {
	IDLE = 0,
	LEFT,
	RIGHT,
	FAINT,
	DEATH
};

class EnemyBat:
	public Object
{
private:
	enum hitPosition
	{
		top,
		bottom,
		left,
		right
	};
	class Camera* camera;	//カメラ情報
	Vector2D velocity;		//コウモリのベクトル
	BatState bat_state;		//コウモリの状態
	bool stageHitFlg[2][4]; //コウモリとステージの当たり判定
	float move[4];			//各方向加速度格納用
	float wing_angle;		//羽の動かす用
	int up;					//sin波で移動する用
	float len;				//ダメージ中の方向保存
	bool wall_hit_flg;		//壁に当たったかどうか
	int wall_hit_timer;		//壁に当たった直後から時間測定
	bool se_once;		//一回だけ再生	
	float faint_timer;	//スタン時間測定
	int death_timer;	//死亡演出

	int bat_image;		//コウモリ画像格納
	int damage_image;	//コウモリダメージ画像格納

	int wing_se;		//羽SE
	int damage_se[3];	//被ダメージSE格納
	int faint_se;		//スタンSE
	int fall_se;		//落下SE
	int hit_se;			//ぶつかるSE

public:
	//コンストラクタ
	EnemyBat();

	//デストラクタ
	~EnemyBat();

	/// <summary>
	/// 初期処理
	/// </summary>
	/// <param name="_location">スポーン座標</param>
	/// <param name="_erea">大きさ</param>
	/// <param name="_color_data">色情報</param>
	void Initialize(Vector2D _location, Vector2D _erea, int _color_data)override;

	/// <summary>
	/// 描画以外の更新
	/// </summary>
	/// <param name="_manager">このオブジェクトを管理するマネージャークラスのポインタ</param>
	void Update(ObjectManager* _manager)override;

	//描画の更新
	void Draw()const override;

	//終了時処理
	void Finalize()override;

public:
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="_manager">このオブジェクトを管理するマネージャークラスのポインタ</param>
	void Move(ObjectManager* _manager);

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
	bool SearchColor(Object* ob) {
		return 0;
	}

	/// <summary>
	/// ステージとの当たり判定
	/// </summary>
	/// <param name="l">対象の座標</param>
	/// <param name="e">対象の大きさ</param>
	/// <returns>当たったかどうか</returns>
	bool CheckCollision(Vector2D l, Vector2D e); 
};

