#include "PadInput.h"
#include "DebugInformation.h"

char PadInput::NowKey[BUTTONS];
char PadInput::OldKey[BUTTONS];
bool PadInput::NowStick[8] = { false };
bool PadInput::OldStick[8] = { false };	
bool PadInput::NowTrigger[2];
bool PadInput::OldTrigger[2];
XINPUT_STATE PadInput::Input;
Stick PadInput::Rstick;
Stick PadInput::Lstick;

int PadInput::PressingTime[R_TRIGGER] = { 0 };

void PadInput::UpdateKey()
{
	// 入力キー取得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &Input);
	for (int i = 0; i < BUTTONS; i++)
	{
		OldKey[i] = NowKey[i];
		NowKey[i] = Input.Buttons[i];
	}

	
	//左スティック
	Lstick.ThumbX = Input.ThumbLX;
	Lstick.ThumbY = Input.ThumbLY;

	//右スティック
	Rstick.ThumbX = Input.ThumbRX;
	Rstick.ThumbY = Input.ThumbRY;

	//トリガー
	OldTrigger[0] = NowTrigger[0];
	NowTrigger[0] = Input.LeftTrigger > 127;

	OldTrigger[1] = NowTrigger[1];
	NowTrigger[1] = Input.RightTrigger > 127;

	//スティックをboolで判断した処理更新
	for (int i = 0; i < 8; i++)
	{
		OldStick[i] = NowStick[i];
		NowStick[i] = GetStick(i + L_STICK_UP);
	}

	//各ボタン押されていたらタイマー加算、押されていなければ0に
	for (int i = 0; i < L_TRIGGER; i++)
	{
		if (OnPressed(i))PressingTime[i]++;
		else PressingTime[i] = 0;
	}
}

bool PadInput::OnButton(int button)
{
	//トリガーは別処理
	if (button >= L_TRIGGER)return (NowTrigger[button - L_TRIGGER] == true && OldTrigger[button - L_TRIGGER] == false);
	//スティックは別処理
	if (button >= L_STICK_UP)return (NowStick[button- L_STICK_UP] == true && OldStick[button- L_STICK_UP] == false);
	//ボタン処理
	bool ret = (NowKey[button] == true && OldKey[button] == false);
	return ret;
}

bool PadInput::OnPressed(int button)
{
	//トリガーは別処理
	if (button >= L_TRIGGER)return NowTrigger[button - L_TRIGGER] == true;
	//スティックは別処理
	if (button >= L_STICK_UP)return NowStick[button- L_STICK_UP] == true;
	//ボタン処理
	bool ret = (NowKey[button] == true);
	return ret;
}

bool PadInput::OnRelease(int button)
{
	//トリガーは別処理
	if (button >= L_TRIGGER)return (NowTrigger[button - L_TRIGGER] == false && OldTrigger[button - L_TRIGGER] == true);
	//スティックは別処理
	if (button >= L_STICK_UP)return (NowStick[button- L_STICK_UP] == false && OldStick[button- L_STICK_UP] == true);
	//ボタン処理
	bool ret = (NowKey[button] == false && OldKey[button] == true);
	return ret;
}

float PadInput::TipLStick(short StickL)
{
	if (StickL == STICKL_X)
	{
		float ratioL_X = Lstick.ThumbX / MAXL_X;

		//左スティックの横軸を最大値を１とした割合
		return ratioL_X;
	}
	else if (StickL == STICKL_Y)
	{
		float ratioL_Y = Lstick.ThumbY / MAXL_Y;

		//左スティックの縦軸値の最大値を１とした割合
		return ratioL_Y;
	}

	return 0;
}

float PadInput::TipRStick(short StickR)
{
	if (StickR == STICKL_X)
	{
		float ratioR_X = Rstick.ThumbX / MAXL_X;
		//右スティックの横軸を最大値を１とした割合
		return ratioR_X;
	}
	else if (StickR == STICKL_Y)
	{
		float ratioR_Y = Rstick.ThumbY / MAXL_Y;
		//右スティックの縦軸値の最大値を１とした割合
		return ratioR_Y;
	}

	return 0;
}

bool PadInput::GetStick(int _assign_key)
{
	//知りたいスティックによって処理を変える
	switch (_assign_key)
	{
		//左スティック上
	case L_STICK_UP:
		return PadInput::TipLStick(STICKL_Y) > 0.5f;
		break;
		//左スティック下
	case L_STICK_DOWN:
		return PadInput::TipLStick(STICKL_Y) < -0.5f;
		break;
		//左スティック右
	case L_STICK_RIGHT:
		return PadInput::TipLStick(STICKL_X) > 0.5f;
		break;
		//左スティック左
	case L_STICK_LEFT:
		return PadInput::TipLStick(STICKL_X) < -0.5f;
		break;
		//右スティック上
	case R_STICK_UP:
		return PadInput::TipRStick(STICKL_Y) > 0.5f;
		break;
		//右スティック下
	case R_STICK_DOWN:
		return PadInput::TipRStick(STICKL_Y) < -0.5f;
		break;
		//右スティック右
	case R_STICK_RIGHT:
		return PadInput::TipRStick(STICKL_X) > 0.5f;
		break;
		//右スティック左
	case R_STICK_LEFT:
		return PadInput::TipRStick(STICKL_X) < -0.5f;
		break;
	default:
		break;
	}
	return false;
}

bool PadInput::GetPressedButton(int _button, int _start_time, int _interval)
{
	//指定したキーが長押しされている時間が、開始時間以上か判断
	if (PressingTime[_button] > _start_time)
	{
		//開始した瞬間を0フレーム目として、それ以降は指定した間隔毎に真を返す
		return ((PressingTime[_button] - _start_time) % _interval == 0);
	}
	//押された瞬間も一回だけ真
	if (PressingTime[_button] == 1)return true;
	return false;
}

int PadInput::GetNowInput()
{
	//ボタンの入力を調べる
	for (int i = 0; i < BUTTONS; i++)
	{
		if (NowKey[i] == 1)return i;
	}
	//スティックの入力を調べる
	for (int j = 0; j < 8; j++)
	{
		if (NowStick[j] == 1)return j + L_STICK_UP;
	}
	//左右トリガーの入力を調べる
	for (int k = 0; k < 2; k++)
	{
		if (NowTrigger[k] == 1)return k + L_TRIGGER;
	}
	//何も入力されていない事を返す
	return -1;
}