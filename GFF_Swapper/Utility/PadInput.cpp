#include "PadInput.h"
#include "DebugInfomation.h"

char PadInput::NowKey[BUTTONS];
char PadInput::OldKey[BUTTONS];
bool PadInput::NowStick[8] = { false };
bool PadInput::OldStick[8] = { false };	
XINPUT_STATE PadInput::Input;
Stick PadInput::Rstick;
Stick PadInput::Lstick;

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

	//スティックをboolで判断した処理更新
	for (int i = 0; i < 8; i++)
	{
		OldStick[i] = NowStick[i];
		NowStick[i] = GetStick(i + L_STICK_UP);
	}
}

bool PadInput::OnButton(int button)
{
	//スティックは別処理
	if (button >= L_STICK_UP)return (NowStick[button- L_STICK_UP] == true && OldStick[button- L_STICK_UP] == false);
	//ボタン処理
	bool ret = (NowKey[button] == 1 && OldKey[button] == 0);
	return ret;
}

bool PadInput::OnPressed(int button)
{
	//スティックは別処理
	if (button >= L_STICK_UP)return NowStick[button- L_STICK_UP] == true;
	//ボタン処理
	bool ret = (NowKey[button] == 1);
	return ret;
}

bool PadInput::OnRelease(int button)
{
	//スティックは別処理
	if (button >= L_STICK_UP)return NowStick[button- L_STICK_UP] == false && OldStick[button- L_STICK_UP] == true;
	//ボタン処理
	bool ret = (NowKey[button] == 0 && OldKey[button] == 1);
	return ret;
}

float PadInput::TipLStick(short StickL)
{
	if (StickL == STICKL_X)
	{
		float ratioL_X = Input.ThumbLX / MAXL_X;

		//左スティックの横軸を最大値を１とした割合
		return ratioL_X;
	}
	else if (StickL == STICKL_Y)
	{
		float ratioL_Y = Input.ThumbLY / MAXL_Y;

		//左スティックの縦軸値の最大値を１とした割合
		return ratioL_Y;
	}

	return 0;
}

float PadInput::TipRStick(short StickR)
{
	if (StickR == STICKL_X)
	{
		float ratioR_X = Input.ThumbRX / MAXL_X;
		//右スティックの横軸を最大値を１とした割合
		return ratioR_X;
	}
	else if (StickR == STICKL_Y)
	{
		float ratioR_Y = Input.ThumbRY / MAXL_Y;
		//右スティックの縦軸値の最大値を１とした割合
		return ratioR_Y;
	}

	return 0;
}

//スティックの傾きをboolで返す
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

int PadInput::GetNowInput()
{
	//ボタンの入力を調べる
	for (int i = 0; i < BUTTONS; i++)
	{
		if (Input.Buttons[i] == 1)return i;
	}
	//スティックの入力を調べる初期値L_STICK_UPから始まり、2つのスティックの上下左右を調べる為、８を足す
	for (int j = L_STICK_UP; j < L_STICK_UP + 8; j++)
	{
		if (GetStick(j))return j;
	}
	//何も入力されていない事を返す
	return -1;
}