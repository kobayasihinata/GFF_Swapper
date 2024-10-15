#include "main.h"
// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int
	nCmdShow)
{
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	SetMainWindowText(APP_NAME);                   // �E�B���h�E�^�C�g����ݒ�
	//SetMainWindowClassName(APP_NAME);              // ���� DxLib �Ƌ������Ȃ��悤�ɃE�B���h�E�N���X��ݒ�
	ChangeWindowMode(TRUE);                        // �E�C���h�E���[�h�ŋN��
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32); // �E�C���h�E�̃T�C�Y
	if (DxLib_Init() == -1) return -1;             // DxLib �̏���������
	SetDrawScreen(DX_SCREEN_BACK);                 // �`����ʂ𗠂ɂ���i�_�u���o�b�t�@�����O�j
	//SetAlwaysRunFlag(TRUE);							//��ɃA�N�e�B�u��Ԃɂ���

	//SoundManager::CreateSoundManager();

	SceneManager* sceneMng = new SceneManager((Scene*) new Title());

	FPSCtrl::SetLimitRate(SCREEN_FPS);
	FPSCtrl::SetUpdateInterval(1000);

	// �Q�[�����[�v���V�[���̍X�V
	while ((ProcessMessage() == 0) && (sceneMng->update() != nullptr)) {
		ClearDrawScreen();

		//if ((GetMainWindowHandle() == GetForegroundWindow()))
		//{
		//	InputCtrl::Update();
		//}
		InputCtrl::Update();
		FPSCtrl::Limit();
		FPSCtrl::Update();

		sceneMng->draw();

		//////////////////////////////////////////////////
		// ��
		//SetFontSize(16);
		//DrawFormatString(10, 10, 0xffffff, "FPS: %0.0f", FPSCtrl::Get());

		if (InputCtrl::GetButtonState(XINPUT_BUTTON_BACK)) break; // �����I��
		//////////////////////////////////////////////////

		ScreenFlip();
	};

	//SoundManager::DeleteSoundManager();

	DxLib_End();
	return 0;
}