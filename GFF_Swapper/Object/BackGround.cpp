#include "DxLib.h"
#include "BackGround.h"
#include "../Utility/ResourceManager.h"

BackGround::BackGround():stage_erea{0.0}, now_stage(0), is_clear(false), mountain_handle(0), cloud_handle(0)
{
	bg_erea.x = 1920;
	bg_erea.y = 720;
}

BackGround::~BackGround()
{

}

void BackGround::Initialize(Vector2D _stage_erea)
{
	stage_erea = _stage_erea;

	back_ground_image = ResourceManager::SetGraph("Resource/Images/sozai/back_image.PNG");
	boss_bg_image = ResourceManager::SetGraph("Resource/Images/sozai/back_image2.PNG");
	font_handle[0] = CreateFontToHandle("Franklin Gothic", 48, -1, -1);
	font_handle[1] = CreateFontToHandle("Wingdings", 48, -1, -1);
	bg_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, TRUE);
}

void BackGround::Finalize()
{

}

void BackGround::Update()
{
	//山の更新
	SetDrawScreen(mountain_handle);
}

void BackGround::Draw(Vector2D _camera_location)const
{
	Vector2D shift_location = { -_camera_location.x / (stage_erea.x / (bg_erea.x / 12)) ,-_camera_location.y / (stage_erea.y / (bg_erea.y / 7.2f))};
	int r = 0, g = 0, b = 0;
	int bg_color = /*(_camera_location.x / 80)*/0;	//背景のバグ演出を一旦消す

	//ボスエリア以外の背景
	if (now_stage != STAGE_NUM - 1 || is_clear)
	{
		DrawGraphF(shift_location.x, 0, ResourceManager::GetGraph(back_ground_image), TRUE);
	}
	//ボスエリアの背景(暗転)
	else if(is_blind)
	{
	
	}
	//ボスエリアの背景(通常)
	else
	{
		DrawGraph(0, 0, ResourceManager::GetGraph(boss_bg_image), TRUE);
	}

#ifdef _DEBUG
	if (!DebugInfomation::GetPhotographMode)
	{
		DrawStringF(shift_location.x, 300.0f, "左端", 0x00ff00);
		DrawStringF((bg_erea.x - 50) + shift_location.x, 300.0f, "右端", 0x00ff00);
	}
#endif
}

void BackGround::DrawWood(Vector2D _shift_location, float move_speed)const
{

}

void BackGround::DrawMountain(Vector2D _shift_location,Vector2D _erea ,float _move_speed, int  _bg_color)const
{
	if(GetRand(_bg_color) > 100)SetDrawBlendMode(DX_BLENDMODE_INVSRC,220);

	Vector2D shift_location = { _shift_location.x * _move_speed,_shift_location.y * _move_speed };
	DrawCircleAA(SCREEN_WIDTH + shift_location.x, SCREEN_HEIGHT - shift_location.y - _erea.y, _erea.x, 50, 0x00aa00, TRUE);
	DrawCircleAA(SCREEN_WIDTH + shift_location.x, SCREEN_HEIGHT - shift_location.y - _erea.y, _erea.x, 50, 0x000000, FALSE);
	DrawBoxAA(SCREEN_WIDTH + shift_location.x - _erea.x,
		SCREEN_HEIGHT - shift_location.y - _erea.y,
		SCREEN_WIDTH + shift_location.x + _erea.x,
		SCREEN_HEIGHT,
		0x00aa00, TRUE);
	DrawLineAA(SCREEN_WIDTH + shift_location.x - _erea.x,
		SCREEN_HEIGHT - shift_location.y - _erea.y,
		SCREEN_WIDTH + shift_location.x - _erea.x,
		SCREEN_HEIGHT,
		0x000000, TRUE);
	DrawLineAA(SCREEN_WIDTH + shift_location.x + _erea.x,
		SCREEN_HEIGHT - shift_location.y - _erea.y,
		SCREEN_WIDTH + shift_location.x + _erea.x,
		SCREEN_HEIGHT,
		0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
#ifdef _DEBUG
	DrawStringF(SCREEN_WIDTH + shift_location.x, SCREEN_HEIGHT - shift_location.y - _erea.y, "山", 0x000000);
#endif
}

void BackGround::DrawCloud(Vector2D _shift_location, Vector2D _erea, float move_speed, int _bg_color)const
{
	Vector2D shift_location = { _shift_location.x * move_speed , _shift_location.y};

	if (GetRand(_bg_color) > 100)SetDrawBlendMode(DX_BLENDMODE_ADD, 220);

	DrawCircleAA(SCREEN_WIDTH + shift_location.x, shift_location.y, _erea.x, 30, 0xaaaaaa, TRUE);
	DrawCircleAA(SCREEN_WIDTH + shift_location.x - 3, shift_location.y-3, _erea.x-1, 30, 0xcccccc, TRUE);
	DrawCircleAA(SCREEN_WIDTH + shift_location.x + 30 + (_erea.x / 5), shift_location.y + 2, _erea.x, 30, 0xaaaaaa, TRUE);
	DrawCircleAA(SCREEN_WIDTH + shift_location.x + 27 + (_erea.x / 5), shift_location.y - 1, _erea.x - 1, 30, 0xcccccc, TRUE);
	DrawCircleAA(SCREEN_WIDTH + shift_location.x + 65 + (_erea.x / 3), shift_location.y - 3, _erea.x, 30, 0xaaaaaa, TRUE);
	DrawCircleAA(SCREEN_WIDTH + shift_location.x + 62 + (_erea.x / 3), shift_location.y - 6, _erea.x - 1, 30, 0xcccccc, TRUE);
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}