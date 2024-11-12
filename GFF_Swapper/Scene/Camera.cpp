#include "Camera.h"
#include "../Utility/PadInput.h"

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

	//カメラのずらし方更新
	CameraShiftUpdate();

	//プレイヤー座標更新
	player_location = _player_location;

	//カメラ座標更新
	camera_location.x = _player_location.x - (SCREEN_WIDTH / 2)+ camera_shift.x;
	camera_location.y = _player_location.y - (SCREEN_HEIGHT / 2)+ camera_shift.y;

	//カメラX座標が画面左端以下なら
	if (camera_location.x <= lock_pos[0].x)
	{
		//カメラのX座標を左端固定する
		camera_location.x = lock_pos[0].x;
	}
	//カメラX座標が画面右端以上なら
	if (camera_location.x >= lock_pos[1].x)
	{
		//カメラのX座標を右端固定する
		camera_location.x = lock_pos[1].x;
	}
	//カメラY座標が画面上端以上なら
	if (camera_location.y <= lock_pos[0].y)
	{
		//カメラのX座標を上端固定する
		camera_location.y = lock_pos[0].y;
	}
	//カメラY座標が画面下端以上なら
	if (camera_location.y >= lock_pos[1].y)
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

void Camera::CameraShiftUpdate()
{
	//右スティックが真ん中以外か判断
	if (PadInput::TipRStick(STICKL_X) > 0.1f ||
		PadInput::TipRStick(STICKL_X) < -0.1f ||
		PadInput::TipRStick(STICKL_Y) > 0.1f ||
		PadInput::TipRStick(STICKL_Y) < -0.1f)
	{
		//スティックの傾き分カメラ移動の値に加算
		camera_shift.x += (PadInput::TipRStick(STICKL_X) * 10);
		camera_shift.y -= (PadInput::TipRStick(STICKL_Y) * 10);
	}
	//スティックが中心に近いなら
	else
	{
		//加算した値を緩やかに0まで戻す
		camera_shift.x -= (camera_shift.x / 10);
		camera_shift.y -= (camera_shift.y / 10);
	}

	//カメラ移動の上限値
	if (camera_shift.x > X_SHITF_LIMIT)camera_shift.x = X_SHITF_LIMIT;
	if (camera_shift.x < -X_SHITF_LIMIT)camera_shift.x = -X_SHITF_LIMIT;
	if (camera_shift.y > Y_SHITF_LIMIT)camera_shift.y = Y_SHITF_LIMIT;
	if (camera_shift.y < -Y_SHITF_LIMIT)camera_shift.y = -Y_SHITF_LIMIT;
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

Vector2D Camera::GetPlayerLocation()const
{
	return player_location;
}