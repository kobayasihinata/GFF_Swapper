#include "Camera.h"
#include "../Utility/PadInput.h"
#include "../Utility/DebugInfomation.h"

Camera* Camera::Get()
{

	static Camera instance;
	//自分自身のポインタを返却する
	return &instance;
}

void Camera::Update(int _now_stage, Vector2D _player_location)
{
	//カメラ更新（ボスステージ）
	if (_now_stage == STAGE_NUM-1)
	{
		camera_location = { 160,40 };
	}
	//カメラ更新（通常ステージ）
	else
	{
		//カメラY座標が初期値の時だけプレイヤーへ移動させ、後はゆっくりプレイヤーを追従するようにする
		if (camera_location.y == 0)
		{
			before_moving_camera.y = _player_location.y - (SCREEN_HEIGHT / 2);
		}

		//プレイヤー座標更新
		player_location = _player_location;

		//カメラのずらし方更新
		CameraShiftUpdate();

		//カメラ座標更新
		camera_location.x = _player_location.x - (SCREEN_WIDTH / 2);
		camera_location.y = before_moving_camera.y - ((before_moving_camera.y - player_location.y + (SCREEN_HEIGHT / 2)) / 10);

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
	camera_location.x += (impact_rand + camera_shift.x);
	camera_location.y += (impact_rand + camera_shift.y);

	//動かす前のカメラ座標を保存
	before_moving_camera.x = camera_location.x - (impact_rand + camera_shift.x);
	before_moving_camera.y = camera_location.y - (impact_rand + camera_shift.y);

	//移動後にもう一度端チェック
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
}

void Camera::CameraShiftUpdate()
{
	//プレイヤーの位置に応じてカメラを動かせる量を変える
	float shift_limit_x = X_SHIFT_LIMIT - GetPlayerEdgeDistance().x;
	float shift_limit_y = Y_SHIFT_LIMIT - GetPlayerEdgeDistance().y;
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
	if (camera_shift.x > shift_limit_x)camera_shift.x = shift_limit_x;
	if (camera_shift.x < -shift_limit_x)camera_shift.x = -shift_limit_x;
	if (camera_shift.y > shift_limit_y)camera_shift.y = shift_limit_y;
	if (camera_shift.y < -shift_limit_y)camera_shift.y = -shift_limit_y;
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

Vector2D Camera::GetPlayerEdgeDistance()const
{
	Vector2D ret = { 0,0 };
	//プレイヤーとカメラの座標差の絶対値を格納
	ret.x = fabsf(before_moving_camera.x + (SCREEN_WIDTH / 2) - player_location.x);
	ret.y = fabsf(before_moving_camera.y + (SCREEN_HEIGHT / 2) - player_location.y);

	//座標の差がカメラ可動域を超えないようにする
	if (ret.x > X_SHIFT_LIMIT)ret.x = X_SHIFT_LIMIT;
	if (ret.y > Y_SHIFT_LIMIT)ret.y = Y_SHIFT_LIMIT;

	return ret;
}