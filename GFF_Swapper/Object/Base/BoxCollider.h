#pragma once
#include "dxlib.h"
#include"../../Utility/common.h"
#include "../../Utility/Vector2D.h"

class BoxCollider
{
protected:
	Vector2D location;			//中心座標
	Vector2D local_location;	//画面上での座標
	Vector2D erea;					//範囲
public:

	//当たり判定 (0,0 = 当たっていない それ以外 = 相手の中心座標)
	bool HitBox(BoxCollider* bCollider);

	//当たり判定(引数が違うバージョン)
	bool CheckCollision(Vector2D l, Vector2D e); 

	//左上座標の取得
	Vector2D GetLocation()const;

	//中心座標の取得
	Vector2D GetCenterLocation()const;

	//ローカル座標の取得
	Vector2D GetLocalLocation()const;

	//直径の取得
	Vector2D GetErea()const;

	//座標のセット
	void SetLocation(Vector2D _location);

	//オブジェクトのワールド座標をスクリーン座標に変換する
	void SetScreenPosition(Vector2D _world_to_screen);
};