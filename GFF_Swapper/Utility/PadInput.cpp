#include "PadInput.h"
#include "DebugInfomation.h"

char PadInput::NowKey[BUTTONS];
char PadInput::OldKey[BUTTONS];
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
}

bool PadInput::OnButton(int button)
{
	bool ret = (NowKey[button] == 1 && OldKey[button] == 0);
	return ret;
}

bool PadInput::OnPressed(int button)
{
	bool ret = (NowKey[button] == 1);
	return ret;
}

bool PadInput::OnRelease(int button)
{
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