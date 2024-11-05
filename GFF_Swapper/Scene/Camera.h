#pragma once
#include "../Utility/Vector2D.h"
#include "../Utility/common.h"
#include "Dxlib.h"

class Camera
{
private:
	Vector2D camera_location;	//カメラ座標
	Vector2D lock_pos[2];       //カメラの固定位置（四隅）
	Vector2D stage_size;       //カメラの固定位置（四隅）
	int impact;                 //画面の振動用(ずらす範囲)
	int impact_rand;            //画面の振動用(実際にずらす数値)

private:
	//コンストラクタをprivateにすることで、
//自クラスのメンバ関数でインスタンスを生成できないようにする
	Camera() = default;

	//コピーガード
	//クラス外でインスタンスを生成して渡すことができないようにする
	Camera(Camera& v) = default;
	Camera& operator = (const Camera& v) = delete;
	~Camera() = default;
public:
	//インスタンスを取得する処理
	static Camera* Get();
public:
	//カメラ更新
	void Update(int _now_stage,Vector2D _player_location);
	
	//カメラ座標取得
	Vector2D GetCameraLocation()const;

	//ステージのサイズ取得
	Vector2D GetStageSize()const;

	//ステージの縦幅と横幅を格納する
	void SetStageSize(Vector2D _size);

	//カメラに振動を与える
	void SetImpact(int _num);
};

