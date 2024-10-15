#include "KeyInput.h"

int KeyInput::NowKey[KEY_MAX];
int KeyInput::OldKey[KEY_MAX];
CURSOR KeyInput::Cursor;
int KeyInput::NowMouse[MOUSE_MAX];
int KeyInput::OldMouse[MOUSE_MAX];

//パッド情報の更新
void KeyInput::UpdateKey() {
	// 入力キー取得
	for (int i = 0; i < KEY_MAX; i++) {
		OldKey[i] = NowKey[i];
		NowKey[i] = CheckHitKey(i);
	}
	for (int i = 0; i < MOUSE_MAX; i++)
	{
		OldMouse[i] = NowMouse[i];
		if (GetMouseInput() == i)
		{
			NowMouse[i] = 1;
		}
		else
		{
			NowMouse[i] = 0;
		}
	}
	GetMousePoint(&Cursor.x, &Cursor.y);
}

bool KeyInput::OnKey(int key) {
	return (NowKey[key] == 1 && OldKey[key] == 0);
}

bool KeyInput::OnPresed(int key) {
	return (NowKey[key] == 1);
}

bool KeyInput::OnRelease(int key) {
	return (NowKey[key] == 0 && OldKey[key] == 1);
}

bool KeyInput::OnMouse(int mouse)
{
	return (NowMouse[mouse] == 1 && OldMouse[mouse] == 0);
}

bool KeyInput::OnPressedMouse(int mouse)
{
	return (NowMouse[mouse] == 1);
}

bool KeyInput::OnReleaseMouse(int mouse)
{
	return (NowMouse[mouse] == 0 && OldMouse[mouse] == 1);
}

CURSOR KeyInput::GetMouseCursor()
{
	return  Cursor;
}