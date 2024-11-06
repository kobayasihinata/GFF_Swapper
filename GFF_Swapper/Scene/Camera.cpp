#include "Camera.h"

Camera* Camera::Get()
{

	static Camera instance;
	//自分自身のポインタを返却する
	return &instance;
}

void Camera::Update(int _now_stage, Vector2D _player_location)
{
	//カメラ更新（ボスステージ）
	if (_now_stage == 2)
	{
		camera_location = { 160,40 };
		return;
	}

	//カメラ座標更新
	camera_location.x = _player_location.x - (SCREEN_WIDTH / 2);
	camera_location.y = _player_location.y - (SCREEN_HEIGHT / 2);

	//X座標が画面左端以下なら
	if (_player_location.x < (SCREEN_WIDTH / 2))
	{
		//カメラのX座標を左端固定する
		camera_location.x = lock_pos[0].x;
	}
	//X座標が画面右端以上なら
	if (_player_location.x > stage_size.x - (SCREEN_WIDTH / 2))
	{
		//カメラのX座標を右端固定する
		camera_location.x = lock_pos[1].x;
	}
	//Y座標が画面上端以上なら
	if (_player_location.y < (SCREEN_HEIGHT / 2))
	{
		//カメラのX座標を上端固定する
		camera_location.y = lock_pos[0].y;
	}
	//Y座標が画面下端以上なら
	if (_player_location.y > stage_size.y - (SCREEN_HEIGHT / 2) - 10)
	{
		//カメラのX座標を下端固定する
		camera_location.y = lock_pos[1].y;
	}

	//カメラ振動処理
	if (impact > 0)
	{
		impact--;
		impact_rand = (GetRand(impact) - (impact / 2));
	}
	else
	{
		impact = 0;
		impact_rand = 0;
	}
	camera_location.x += impact_rand;
	camera_location.y += impact_rand;
}

Vector2D Camera::GetCameraLocation()const
{
	return camera_location;
}

Vector2D Camera::GetStageSize()const
{
	return stage_size;
}

void Camera::SetStageSize(Vector2D _size)
{
	stage_size = _size;
	//カメラ固定位置の再設定
	lock_pos[0].x = 0;
	lock_pos[0].y = 0;
	lock_pos[1].x = stage_size.x - SCREEN_WIDTH;
	lock_pos[1].y = stage_size.y - SCREEN_HEIGHT;
}

void Camera::SetImpact(int _num)
{
	impact = _num;
}