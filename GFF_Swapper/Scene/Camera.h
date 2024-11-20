#pragma once
#include "../Utility/Vector2D.h"
#include "../Utility/common.h"
#include "Dxlib.h"

#define X_SHITF_LIMIT 500.0f	//カメラのX可動域（半径）
#define Y_SHITF_LIMIT 300.0f	//カメラのY可動域（半径）

class Camera
{
private:
	Vector2D camera_location = { 0,0 };		//カメラ座標
	Vector2D lock_pos[2];					//カメラの固定位置（四隅）
	Vector2D stage_size;					//カメラの固定位置（四隅）
	Vector2D player_location;				//プレイヤー座標格納用
	Vector2D camera_shift = { 0,0 };		//カメラをずらす
	float camera_size;						//カメラの大きさ
	int impact;								//画面の振動用(ずらす範囲)
	int impact_rand;						//画面の振動用(実際にずらす数値)

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
	void Update(int _now_stage, Vector2D _player_location);
	
	//カメラのずらす値更新
	void CameraShiftUpdate();

	//カメラ座標取得
	Vector2D GetCameraLocation()const;

	//ステージのサイズ取得
	Vector2D GetStageSize()const;

	//ステージの縦幅と横幅を格納する
	void SetStageSize(Vector2D _size);

	//カメラに振動を与える
	void SetImpact(int _num);

	//プレイヤー座標取得
	Vector2D GetPlayerLocation()const;
};