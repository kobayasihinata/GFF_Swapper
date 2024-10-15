//////////////////////////////////////////////////
// 入力 コントローラー
// 作成：kanaaa224
//////////////////////////////////////////////////
#include "inputCtrl.h"

char InputCtrl::nowBtn[PAD_BUTTONS];
char InputCtrl::oldBtn[PAD_BUTTONS];

XINPUT_STATE InputCtrl::padInput;

PadStick InputCtrl::padLStick;
PadStick InputCtrl::padRStick;

int InputCtrl::nowKey[KEYBOARD_KEYS];

MousePoint InputCtrl::mousePoint;
int InputCtrl::nowMouseClick[2];
int InputCtrl::oldMouseClick[2];

void InputCtrl::Update() {
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &padInput);
	for (int i = 0; i < PAD_BUTTONS; i++) {
		oldBtn[i] = nowBtn[i];
		nowBtn[i] = padInput.Buttons[i];
	};

	padLStick.x = padInput.ThumbLX;
	padLStick.y = padInput.ThumbLY;

	padRStick.x = padInput.ThumbRX;
	padRStick.y = padInput.ThumbRY;

	for (int i = 0; i < KEYBOARD_KEYS; i++) {
		if (nowKey[i] <= -1) nowKey[i] = 0;
	};
	GetHitKeyStateAllEx(nowKey);

	GetMousePoint(&mousePoint.x, &mousePoint.y);

	for (int i = 0; i < 2; i++) {
		oldMouseClick[i] = nowMouseClick[i];
		if ((GetMouseInput() & (i + 1)) != 0) nowMouseClick[i] = 1;
		else nowMouseClick[i] = 0;
	};
};

int InputCtrl::GetButtonState(int button) {
	if (nowBtn[button] == 1 && oldBtn[button] == 0)      return 1;
	else if (nowBtn[button] == 1)                        return 2;
	else if (nowBtn[button] == 0 && oldBtn[button] == 1) return 3;
	return 0;
};

PadStick InputCtrl::GetStickState(int stickType) {
	if (stickType) return padRStick;
	return padLStick;
};

PadStick InputCtrl::GetStickRatio(int stickType) {
	PadStick stickRatio;
	stickRatio.x = padLStick.x / STICK_MAX;
	stickRatio.y = padLStick.y / STICK_MAX;
	if (stickType) {
		stickRatio.x = padRStick.x / STICK_MAX;
		stickRatio.y = padRStick.y / STICK_MAX;
	};
	stickRatio.x = round(stickRatio.x * 100.0f) / 100.0f;
	stickRatio.y = round(stickRatio.y * 100.0f) / 100.0f;
	return stickRatio;
};

int InputCtrl::GetKeyState(int key) {
	if (nowKey[key] == 1)       return 1;
	else if (nowKey[key] >= 1)  return 2;
	else if (nowKey[key] <= -1) return 3;
	return 0;
};

int InputCtrl::GetMouseState(int button) {
	if (nowMouseClick[button] == 1 && oldMouseClick[button] == 0)      return 1;
	else if (nowMouseClick[button] == 1)                               return 2;
	else if (nowMouseClick[button] == 0 && oldMouseClick[button] == 1) return 3;
	return 0;
};

MousePoint InputCtrl::GetMouseCursor() {
	return mousePoint;
};