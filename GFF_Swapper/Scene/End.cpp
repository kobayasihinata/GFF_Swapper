#include "End.h"
#include "DxLib.h"
#include <fstream>
#include "../Utility/PadInput.h"


End::End() :
	shift_y(0),
	stop_time(0),
	boss_anim(0),
	boss_color(0),
	boss_cnt(0),
	up(0),
	fast_forward(0),
	deer_speed(0),
	face_angle(0.0f),
	scroll_speed(0),
	frog_speed{ 0.0f,0.0f },
	bat_loction{ 0.0f,0.0f },
	deer_location{ 0.0f,0.0f },
	frog_location{ 0.0f, 0.0f },
	frog_erea{ 0.0f,0.0f },
	boss_location{ 0.0f,0.0f },
	player_location{ 0.0f,0.0f },
	player_erea{ 0.0f,0.0f },
	end_game_flg(false)
{
	for (int i = 0; i < 7; i++)
	{
		logo_location[i].x = 0;
	}

	for (int i = 0; i < 4; i++) {
		leg_angle[i] = 0.0f;
	}

	for (int i = 0; i < 4; i++) {
		player_angle[i] = 0;
	}
	// wing の初期化
	wing.fill({ 0.0f,0.0f });
	wing_mirror.fill({ 0.0f,0.0f });

	boss_color = 0;
	for (int i = 0; i < BG_BLOCK_WIDTH_NUM; i++)
	{
		for (int j = 0; j < BG_BLOCK_HEIGHT_NUM; j++)
		{
			bg[i][j].flg = true;
			bg[i][j].location = { 0,0 };
			bg[i][j].erea = { 0,0 };
			bg[i][j].move_flg = false;
			bg[i][j].move_goal = { 0,0 };
			bg[i][j].move_speed = 0;
			bg[i][j].color = 0;
			bg[i][j].move_rad = 0.0f;
			bg[i][j].anim_size = 0;
		}
	}
	t = new Title();
}

End::~End()
{
}

void End::Initialize()
{
	for (int i = 0; i < 7; i++)
	{
		logo_location[i] = { 0,0 };
		for (int j = 0; j < i; j++)
		{
			logo_location[i].x += 40;
		}
	}

	player_location = { 1300.f, 480.f };
	player_erea = { PLAYER_HEIGHT,PLAYER_WIDTH };

	bat_loction = { 100.f, 200.f };

	deer_location = { 1280.f, 500.f };

	frog_location = { -40.f,600.f };
	frog_erea = { 50.f,50.f };

	boss_location = { 1315.f,-150.f };

	deer_speed = 800.f;
	frog_speed = {};

	scroll_speed = 3;
	shift_y = -600;

	fast_forward = 1;

	for (int i = 0; i < BG_BLOCK_WIDTH_NUM; i++)
	{
		for (int j = 0; j < BG_BLOCK_HEIGHT_NUM; j++)
		{
			bg[i][j].flg = true;
			bg[i][j].location = { (float)i * 40,(float)j * 40 };
			bg[i][j].erea = { 40,40 };
			bg[i][j].move_flg = false;
			bg[i][j].move_goal = { 0,0 };
			bg[i][j].move_speed = 6;
			bg[i][j].color = t->GetRandColor();
			bg[i][j].move_rad = 0.0f;
			bg[i][j].anim_size = 0;
		}
	}

	int xnum = (SCREEN_WIDTH / cellSize_) + 1;
	int ynum = (SCREEN_HEIGHT / cellSize_) + 1;
	tiles_.reserve(xnum * ynum);

	for (int yidx = 0; yidx < ynum; ++yidx) {
		for (int xidx = 0; xidx < xnum; ++xidx) {
			tiles_.push_back({ xidx,yidx });
		}
	}
	std::shuffle(tiles_.begin(), tiles_.end(), mt_);

	LoadPosition();  // 初期化時に座標を読み込む

	swap_se = ResourceManager::SetSound("Resource/Sounds/Effect/swap.wav");

}

void End::Finalize()
{

}

AbstractScene* End::Update()
{
	scroll_speed = 3;
	fast_forward = 1;
	if (PadInput::OnPressed(XINPUT_BUTTON_B)) {
		scroll_speed = 15;
		fast_forward = 4;
		stop_time += 3;
		if (shift_y >= 2400) {
			ExitNum += 3;
		}
	}
	up++;
	//タイトルロゴが止まるように
	if (shift_y > -80 || shift_y > 2400) {
		stop_time++;
	}
	else {
		shift_y += scroll_speed;
	}

	//タイトルロゴが止まってから一定時間たったら動くように
	if (stop_time > 120 && shift_y < 2400) {
		shift_y += scroll_speed;
	}

	//thank you for playingが止まるように
	if (shift_y >= 2400) {
		ExitNum++;
	}
	//終了処理
	if (ExitNum > 179 && !end_game_flg)
	{
		end_game_flg = true;
		end_image_handle = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
		GetDrawScreenGraph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, end_image_handle);
	}

	if (end_game_flg == true)
	{
		EndGameUpdate();
		if (end_anim_count > 90)
		{
			return nullptr;
		}
	}
	DeerUpdate();
	BatUpdate();
	FrogUpdate();
	BossUpdate();
	PlayerUpdate();

	return this;
}

void End::Draw()const
{
	if (end_game_flg)
	{
		//DrawGraph(0, 0, title_image_handle, TRUE);
		auto rate = (float)end_anim_count / (float)interval_;
		for (const auto& cell : tiles_) {
			DrawRectGraph(
				cell.xidx * cellSize_,
				cell.yidx * cellSize_,
				cell.xidx * cellSize_,
				cell.yidx * cellSize_,
				cellSize_, cellSize_,
				end_image_handle, true);
			DrawBox(cell.xidx * cellSize_,
				cell.yidx * cellSize_,
				cell.xidx * cellSize_ + cellSize_,
				cell.yidx * cellSize_ + cellSize_,
				0xffffff, FALSE);
		}
	}
	else
	{
		//背景
		BackGroundDraw();
		//コウモリ
		BatDraw();
		//シカ
		DeerDraw();
		//カエル
		FrogDraw();
		//ボス
		BossDraw();
	//背景
	BackGroundDraw();
	//コウモリ
	BatDraw();
	//シカ
	DeerDraw();
	//カエル
	FrogDraw();
	//ボス
	BossDraw();
	//プレイヤー
	PlayerDraw();


		//エンドロール
		SetFontSize(60);
		//DrawString((SCREEN_WIDTH / 2) - 105, 200 - shift_y, "TITLE", 0xffffff);
		DrawString(435, 240 - shift_y, "「", 0xffffff);
		DrawString(780, 255 - shift_y, "」", 0xffffff);
		for (int i = 0; i < 7; i++)
		{
			DrawFormatStringF(logo_location[i].x + 500, logo_location[i].y + 250 - shift_y, logo_color[i], "%s", logo_string[i]);
		}

		DrawString(340, 2400 + 300 - shift_y, "Thank     for playing", 0xffffff);
		DrawString(530, 2400 + 300 - shift_y, "y", 0xff0000);
		DrawString(560, 2400 + 300 - shift_y, "o", 0x0000ff);
		DrawString(590, 2400 + 300 - shift_y, "u", 0x00ff00);


		SetFontSize(30);
		DrawString(500, 500 + 300 - shift_y, "Production Members", 0xff0000);
		DrawString(530, 600 + 300 - shift_y, "Hiroki Shinzato", 0xffffff);
		DrawString(530, 640 + 300 - shift_y, "Hayato Kitamura", 0xffffff);
		DrawString(530, 680 + 300 - shift_y, "Hinata Kobayashi", 0xffffff);
		DrawString(530, 720 + 300 - shift_y, "Reo Yamaguchi", 0xffffff);

		DrawString(505, 1020 + 300 - shift_y, "Site of Music Used", 0x0000ff);
		DrawString(535, 1120 + 300 - shift_y, "「無料効果音」", 0xffffff);
		DrawString(535, 1170 + 300 - shift_y, "「効果音ラボ」", 0xffffff);
		DrawString(515, 1220 + 300 - shift_y, "「アルスパーク」", 0xffffff);

		DrawString(550, 1380 + 300 - shift_y, "Title Music", 0x00ff00);
		DrawString(450, 1460 + 300 - shift_y, "sound jewel 「Good Night」", 0xffffff);

		DrawString(520, 1630 + 300 - shift_y, "GameMain Music", 0x00ff00);
		DrawString(400, 1710 + 300 - shift_y, "Dynamedion 「A Chill in the air」", 0xffffff);
	}
}

void End::BackGroundDraw()const
{
	//動いていないブロックを先に描画
	for (int i = 0; i < BG_BLOCK_WIDTH_NUM; i++)
	{
		for (int j = 0; j < BG_BLOCK_HEIGHT_NUM; j++)
		{
			if (bg[i][j].move_flg == false && bg[i][j].flg == true)
			{
				DrawBoxAA(bg[i][j].location.x - bg[i][j].anim_size,
					bg[i][j].location.y - bg[i][j].anim_size,
					bg[i][j].location.x + bg[i][j].erea.x + bg[i][j].anim_size,
					bg[i][j].location.y + bg[i][j].erea.y + bg[i][j].anim_size, bg[i][j].color, TRUE);
				DrawBoxAA(bg[i][j].location.x - bg[i][j].anim_size,
					bg[i][j].location.y - bg[i][j].anim_size,
					bg[i][j].location.x + bg[i][j].erea.x + bg[i][j].anim_size,
					bg[i][j].location.y + bg[i][j].erea.y + bg[i][j].anim_size, 0x444444, FALSE);
			}
		}
	}
}

void End::PlayerDraw()const
{
	//頭
	ResourceManager::DrawRotaBox(player_location.x - (player_erea.x / 2), player_location.y - (player_erea.y) + 76, 23, 15, player_location.x, player_location.y, 0, GREEN, true);
	ResourceManager::DrawRotaBox(player_location.x - (player_erea.x / 2), player_location.y - (player_erea.y) + 76, 23, 15, player_location.x, player_location.y, 0, 0x000000, false);

	//目
	ResourceManager::DrawRotaBox(player_location.x - (player_erea.x / 2) + 6, player_location.y - (player_erea.y) + 76, 6, 7, player_location.x, player_location.y, 0, 0x000000, true);

	//首
	ResourceManager::DrawRotaBox(player_location.x - (player_erea.x / 2), player_location.y - (player_erea.y) + 62, 10, 5, player_location.x, player_location.y, 0, GREEN, true);
	ResourceManager::DrawRotaBox(player_location.x - (player_erea.x / 2), player_location.y - (player_erea.y) + 62, 10, 5, player_location.x, player_location.y, 0, 0x000000, false);

	//胴体
	ResourceManager::DrawRotaBox(player_location.x - (player_erea.x / 2), player_location.y - (player_erea.y) + 37, 21, 37, player_location.x, player_location.y, 0, GREEN, true);
	ResourceManager::DrawRotaBox(player_location.x - (player_erea.x / 2), player_location.y - (player_erea.y) + 37, 21, 37, player_location.x, player_location.y, 0, 0x000000, false);

	//バッグ
	ResourceManager::DrawRotaBox(player_location.x - (player_erea.x / 2) - 15, player_location.y - (player_erea.y) + 40, 5, 23, player_location.x, player_location.y, 0, GREEN, true);
	ResourceManager::DrawRotaBox(player_location.x - (player_erea.x / 2) - 15, player_location.y - (player_erea.y) + 40, 5, 23, player_location.x, player_location.y, 0, GREEN, false);
	ResourceManager::DrawRotaBox(player_location.x - (player_erea.x / 2) - 15, player_location.y - (player_erea.y) + 40, 3, 15, player_location.x, player_location.y, 0, GREEN, true);

	//腕
	ResourceManager::DrawRotaBox(player_location.x + 15, player_location.y + 55, 28, 7, player_location.x + 25, player_location.y + 55, 20 + 180, GREEN, true);
	ResourceManager::DrawRotaBox(player_location.x + 15, player_location.y + 55, 28, 7, player_location.x + 25, player_location.y + 55, 20 + 180, 0x000000, false);

	ResourceManager::DrawRotaBox(player_location.x + 30, player_location.y + 70, 7, 27, player_location.x + 30, player_location.y + 80, 20, GREEN, true);
	ResourceManager::DrawRotaBox(player_location.x + 30, player_location.y + 70, 7, 27, player_location.x + 30, player_location.y + 80, 20, 0x000000, false);

	//帽子　中央
	DrawTriangleAA(player_location.x + (player_erea.x / 2), player_location.y, player_location.x + 20, player_location.y + 20, player_location.x + 40, player_location.y + 20, GREEN, true);
	DrawTriangleAA(player_location.x + (player_erea.x / 2), player_location.y, player_location.x + 20, player_location.y + 20, player_location.x + 40, player_location.y + 20, 0x000000, false);
	//帽子　右側
	DrawTriangleAA(player_location.x + (player_erea.x / 2), player_location.y, player_location.x + 40, player_location.y + 20, player_location.x + 52, player_location.y + 15, GREEN, true);
	DrawTriangleAA(player_location.x + (player_erea.x / 2), player_location.y, player_location.x + 40, player_location.y + 20, player_location.x + 52, player_location.y + 15, 0x000000, false);
	//帽子　左側
	DrawTriangleAA(player_location.x + (player_erea.x / 2), player_location.y, player_location.x + 8, player_location.y + 15, player_location.x + 20, player_location.y + 20, GREEN, true);
	DrawTriangleAA(player_location.x + (player_erea.x / 2), player_location.y, player_location.x + 8, player_location.y + 15, player_location.x + 20, player_location.y + 20, 0x000000, false);
}

void End::PlayerUpdate()
{
	player_location.x -= 4 * fast_forward;
}

void End::DeerDraw() const
{

	//頭
	ResourceManager::DrawRotaBox(deer_location.x + 16.0f, deer_location.y + 10.0f, 30.0f, 20.0f, deer_location.x + 16.0f, deer_location.y + 10.0f, 0, GREEN, true);
	//目
	ResourceManager::DrawRotaBox(deer_location.x + 8.0f, deer_location.y + 10.0f, 8.0f, 9.0f, deer_location.x + 8.0f, deer_location.y + 10.0f, 0, 0x000000, true);

	ResourceManager::DrawRotaBox(deer_location.x + 25.0f, deer_location.y + 38.0f, 13.0f, 24.0f, deer_location.x + 25.0f, deer_location.y + 38.0f, 0.f, GREEN, true);

	//胴体 vr.1
	ResourceManager::DrawRotaBox(deer_location.x + 53.0f, deer_location.y + 63.0f, 65.0f, 15.0f, deer_location.x + 53.0f, deer_location.y + 63.0f, 0.f, GREEN, true);

	//首 vr.2
	ResourceManager::DrawRotaBox(deer_location.x + 25.0f, deer_location.y + 38.0f, 13.0f, 24.0f, deer_location.x + 25.0f, deer_location.y + 38.0f, 0.f, GREEN, true);

	//胴体 vr.2
	ResourceManager::DrawRotaBox(deer_location.x + 34.0f, deer_location.y + 63.0f, 30.0f, 15.0f, deer_location.x + 34.0f, deer_location.y + 63.0f, 0.f, GREEN, true);
	ResourceManager::DrawRotaBox(deer_location.x + 72.0f, deer_location.y + 63.0f, 30.0f, 15.0f, deer_location.x + 72.0f, deer_location.y + 63.0f, 0.f, GREEN, true);

	//足　左から
	ResourceManager::DrawRotaBox(deer_location.x + 27.0f - 4, deer_location.y + 88.0f, 10.0f, 25.0f, deer_location.x + 27.0f - 4, deer_location.y + 88.0f, leg_angle[0], GREEN, true);
	ResourceManager::DrawRotaBox(deer_location.x + 43.0f, deer_location.y + 88.0f, 10.0f, 25.0f, deer_location.x + 43.0f, deer_location.y + 88.0f, leg_angle[1], GREEN, true);
	//ResourceManager::DrawRotaBox(deer_location.x + 35.0f + d_left_leg[1], deer_location.y + 88.0f, 10.0f, 25.0f, deer_location.x + 35.0f, deer_location.y + 88.0f, -leg_angle[1], draw_color, true);
	ResourceManager::DrawRotaBox(deer_location.x + 68.0f - 4, deer_location.y + 88.0f, 10.0f, 25.0f, deer_location.x + 68.0f - 4, deer_location.y + 88.0f, leg_angle[2], GREEN, true);
	ResourceManager::DrawRotaBox(deer_location.x + 83.0f, deer_location.y + 88.0f, 10.0f, 25.0f, deer_location.x + 83.0f, deer_location.y + 88.0f, leg_angle[3], GREEN, true);
	//ResourceManager::DrawRotaBox(local_location.x + 75.0f + d_left_leg[3], local_location.y + 88.0f, 10.0f, 25.0f, local_location.x + 75.0f, local_location.y + 88.0f, -leg_angle[3], draw_color, true);
}

void End::DeerUpdate()
{
	deer_location.x -= 4 * fast_forward;
	// 足の回転方向を制御するフラグを追加
	for (int i = 0; i < 4; i++) {
		// 回転方向に基づいて角度を更新
		//leg_angle[i] += deer_speed;
		leg_angle[i] -= deer_speed;

		//// 角度の範囲を制限する
		//if (leg_angle[i] >= 25.0f) {
		//	leg_angle[i] = 25.0f - 1;
		//	//回転方向を反転
		//	deer_speed = -deer_speed;
		//}
		//if (leg_angle[i] <= -5.0f) {
		//	leg_angle[i] = -5.0f + 1;
		//	//回転方向を反転
		//	deer_speed = -deer_speed;
		//}
	}
}

void End::BatDraw() const
{
	float wing_angle = (float)sin(PI * 2.f / 40.f * up) * 20.f; // 30度の振れ幅で周期的に変化させる

	const std::vector<Vector2D> vertices = {
		// 耳
		{bat_loction.x + 46, bat_loction.y}, {bat_loction.x + 46, bat_loction.y + 19}, {bat_loction.x + 55, bat_loction.y + 9},
		{bat_loction.x + 69, bat_loction.y}, {bat_loction.x + 69, bat_loction.y + 19}, {bat_loction.x + 60, bat_loction.y + 9},
		//右翼
		{bat_loction.x + 66, bat_loction.y + 33}, {bat_loction.x + 97, bat_loction.y + 7}, {bat_loction.x + 117, bat_loction.y + 65},
		{bat_loction.x + 66, bat_loction.y + 33}, {bat_loction.x + 97, bat_loction.y + 7}, {bat_loction.x + 96, bat_loction.y + 61},
		{bat_loction.x + 65, bat_loction.y + 33}, {bat_loction.x + 97, bat_loction.y + 7}, {bat_loction.x + 80, bat_loction.y + 57},
		//左翼
		{bat_loction.x + 48, bat_loction.y + 33}, {bat_loction.x + 20, bat_loction.y + 7}, {bat_loction.x - 2, bat_loction.y + 65},
		{bat_loction.x + 48, bat_loction.y + 33}, {bat_loction.x + 20, bat_loction.y + 7}, {bat_loction.x + 18, bat_loction.y + 61},
		{bat_loction.x + 50, bat_loction.y + 33}, {bat_loction.x + 20, bat_loction.y + 7}, {bat_loction.x + 34, bat_loction.y + 57},
		// 頭
		{bat_loction.x + 57, bat_loction.y + 12}, {bat_loction.x + 69, bat_loction.y + 23}, {bat_loction.x + 57, bat_loction.y + 30}, {bat_loction.x + 46, bat_loction.y + 23},
		// 胴体
		{bat_loction.x + 57, bat_loction.y + 34}, {bat_loction.x + 69, bat_loction.y + 52}, {bat_loction.x + 57, bat_loction.y + 73}, {bat_loction.x + 46, bat_loction.y + 52},
	};

	//配列の各頂点を利用して三角形を描画する
	for (int i = 0; i + 2 < vertices.size(); i += 3) {
		//耳
		if (i < 5) {
			DrawTriangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, RED, TRUE);
			DrawTriangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, 0x000000, FALSE);
		}
		//右羽
		else if (i >= 6 && i < 14) {
			// 羽の動き
			DrawLineAA(vertices[7].x, vertices[7].y + wing_angle, vertices[8].x + wing_angle, vertices[8].y, 0x000000);
			DrawLineAA(vertices[6].x, vertices[6].y - 2, vertices[7].x, vertices[7].y - 2 + wing_angle, 0x000000);
			//DrawTriangleAA(vertices[i].x - 1, vertices[i].y - 1, vertices[i + 1].x - 1, vertices[i + 1].y - 1 + wing_angle, vertices[i + 2].x + wing_angle - 1, vertices[i + 2].y - 1, 0x000000, FALSE);
			DrawTriangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y + wing_angle, vertices[i + 2].x + wing_angle, vertices[i + 2].y, RED, TRUE);

		}
		//左羽
		else if (i >= 15 && i < 23) {
			// 羽の動き
			DrawLineAA(vertices[16].x, vertices[16].y - 2 + wing_angle, vertices[17].x - wing_angle, vertices[17].y, 0x000000);
			DrawLineAA(vertices[15].x, vertices[15].y - 2, vertices[16].x, vertices[16].y - 2 + wing_angle, 0x000000);
			//DrawTriangleAA(vertices[i].x + 1, vertices[i].y + 1, vertices[i + 1].x + 1, vertices[i + 1].y - 1 + wing_angle, vertices[i + 2].x - 3 - wing_angle, vertices[i + 2].y - 3, 0x000000, FALSE);
			DrawTriangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y + wing_angle, vertices[i + 2].x - wing_angle, vertices[i + 2].y, RED, TRUE);

		}
		//ひし形の描画
		else if (i + 3 < vertices.size())
		{
			DrawQuadrangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, vertices[i + 3].x, vertices[i + 3].y, RED, TRUE);
			DrawQuadrangleAA(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, vertices[i + 2].x, vertices[i + 2].y, vertices[i + 3].x, vertices[i + 3].y, 0x000000, FALSE);
			i++;
		}
	}
}

void End::BatUpdate()
{
	bat_loction.x += 2.f * fast_forward;
	bat_loction.y += (float)sin(PI * 2.f / 40.f * up) * 5.f;
}

void End::FrogDraw() const
{
	//胴体
	ResourceManager::DrawRotaBox(frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), frog_erea.x, frog_erea.y / 2, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, BLUE, TRUE);
	ResourceManager::DrawRotaBox(frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), frog_erea.x, frog_erea.y / 2, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, 0x000000, FALSE);

	//右着地
	if (face_angle == 0 && frog_speed.x == 0 && frog_speed.y == 0)
	{
		//付け根側後ろ足
		ResourceManager::DrawRotaBox(frog_location.x + frog_erea.x - 10, frog_location.y + 20, 30, 10, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, BLUE, TRUE);
		ResourceManager::DrawRotaBox(frog_location.x + frog_erea.x - 10, frog_location.y + 20, 30, 10, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, 0x000000, FALSE);
		//後ろ足先端
		ResourceManager::DrawRotaBox(frog_location.x + frog_erea.x - 10, frog_location.y + 10, 40, 10, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, BLUE, TRUE);
		ResourceManager::DrawRotaBox(frog_location.x + frog_erea.x - 10, frog_location.y + 10, 40, 10, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, 0x000000, FALSE);
		//目
		ResourceManager::DrawRotaBox(frog_location.x, frog_location.y + frog_erea.y - 20, 10, 10, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, 0xffffff, TRUE);

	}
	else {
		//付け根側後ろ足
		ResourceManager::DrawRotaBox(frog_location.x + frog_erea.x, frog_location.y + 20, 30, 10, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, BLUE, TRUE);
		ResourceManager::DrawRotaBox(frog_location.x + frog_erea.x, frog_location.y + 20, 30, 10, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, 0x000000, FALSE);
		//後ろ足先端
		ResourceManager::DrawRotaBox(frog_location.x + frog_erea.x + 30, frog_location.y + 10, 40, 10, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, BLUE, TRUE);
		ResourceManager::DrawRotaBox(frog_location.x + frog_erea.x + 30, frog_location.y + 10, 40, 10, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, 0x000000, FALSE);
		//前足
		ResourceManager::DrawRotaBox(frog_location.x + 10, frog_location.y + 10, 10, 15, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, BLUE, TRUE);
		ResourceManager::DrawRotaBox(frog_location.x + 10, frog_location.y + 10, 10, 15, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, 0x000000, FALSE);
		//目
		ResourceManager::DrawRotaBox(frog_location.x, frog_location.y + frog_erea.y - 20, 10, 10, frog_location.x + (frog_erea.x / 2), frog_location.y + (frog_erea.y / 2), face_angle, 0xffffff, TRUE);
	}
}

void End::FrogUpdate()
{
	if (up % 60 == 0) {
		frog_speed.x = 3.f * fast_forward;
		frog_speed.y = -20.f;
	}

	if (frog_speed.x == 0 && frog_speed.y == 0) {
		face_angle = 0;
	}
	else{
		face_angle = atanf(frog_speed.y / frog_speed.x) * 60;
	}

	if (frog_location.y < 600.f) {
		frog_speed.y += 1.f;
		if (frog_speed.y > 16.f) {
			frog_speed.y = 16.f;
		}
	}
	frog_location.x += frog_speed.x;
	frog_location.y += frog_speed.y;
}

void End::BossDraw() const
{


	DrawWing();
	//本体
	DrawCircleAA(boss_location.x + 250 / 2, boss_location.y + 250 / 2 + boss_anim, 35, 35, 0x000000, TRUE);
	DrawCircleAA(boss_location.x + 250 / 2, boss_location.y + 250 / 2 + boss_anim, 35, 34, 0xFFFFFF, FALSE, 3.0f);
	//DrawCircleAA(local_location.x + BOSS_SIZE / 2, local_location.y + BOSS_SIZE / 2 + boss_anim, 36, 36, , FALSE, 2.0f);

	// バリアの描画
	DrawHexagonSphere();
	//for (int i = 0; i < barrier_num; i++) {
	DrawCircleAA(boss_location.x + 250 / 2, boss_location.y + 250 / 2 + boss_anim, 115, 50, boss_color, FALSE, 3.0f);
	DrawCircleAA(boss_location.x + 250 / 2, boss_location.y + 250 / 2 + boss_anim, 112.5, 50, boss_color, FALSE, 2.0f);
	DrawCircleAA(boss_location.x + 250 / 2, boss_location.y + 250 / 2 + boss_anim, 109, 50, boss_color, FALSE);
}

void End::DrawWing() const
{
	Vector2D center = { boss_location.x + 250 / 2, boss_location.y + 250 / 2 };

	// アニメーションに基づいて描画位置を計算
	float angle = 0.0f;

	// 羽の描画（）
	for (int i = 0; i < wing.size(); i += 4) {
		float delta_y = 0.f;
		delta_y = (float)sin(PI * 2.f / 60.f * up + i) * 5.f; // 2番目の羽は中程度に動く
		angle = (float)(sin(PI * 2.f / 60.f * up) * 5.f); // アニメーションに適した角度を計算

		//３番目の羽だけ違う挙動に
		if (i > 7 && i < 12) {
			delta_y = (float)sin(PI * 2.f / 60.f * (float)up + i) * 15.f;
		}
		else if (i > 19 && i < 24) {
			delta_y = (float)sin(PI * 2.f / 60.f * (float)up + i) * 15.f;
		}
		else if (i > 31 && i < 36) {
			delta_y = (float)sin(PI * 2.f / 60.f * (float)up + i) * 15.f;
		}

		//右羽
		DrawQuadrangleAA(boss_location.x + wing[i].x + angle, boss_location.y + wing[i].y + angle + delta_y,
			boss_location.x + wing[i + 1].x + 20 + angle, boss_location.y + wing[i + 1].y + 10 + angle + delta_y,
			boss_location.x + wing[i + 2].x + 10 + angle, boss_location.y + wing[i + 2].y + 20 + angle + delta_y,
			boss_location.x + wing[i + 3].x + angle, boss_location.y + wing[i + 3].y + 30 + angle + delta_y, 0x000000, TRUE);
		//左羽
		DrawQuadrangleAA(boss_location.x + wing_mirror[i].x + 250 - angle, boss_location.y + wing_mirror[i].y + angle + delta_y,
			boss_location.x + wing_mirror[i + 1].x - 20 + 250 - angle, boss_location.y + wing_mirror[i + 1].y + 10 + angle + delta_y,
			boss_location.x + wing_mirror[i + 2].x - 10 + 250 - angle, boss_location.y + wing_mirror[i + 2].y + 20 + angle + delta_y,
			boss_location.x + wing_mirror[i + 3].x + 250 - angle, boss_location.y + wing_mirror[i + 3].y + 30 + angle + delta_y, 0x000000, TRUE);

		//右羽
		DrawQuadrangleAA(boss_location.x + wing[i].x + angle, boss_location.y + wing[i].y + angle + delta_y,
			boss_location.x + wing[i + 1].x + 20 + angle, boss_location.y + wing[i + 1].y + 10 + angle + delta_y,
			boss_location.x + wing[i + 2].x + 10 + angle, boss_location.y + wing[i + 2].y + 20 + angle + delta_y,
			boss_location.x + wing[i + 3].x + angle, boss_location.y + wing[i + 3].y + 30 + angle + delta_y, 0xFFFFFF, FALSE, 3.0f);
		//左羽
		DrawQuadrangleAA(boss_location.x + wing_mirror[i].x + 250 - angle, boss_location.y + wing_mirror[i].y + angle + delta_y,
			boss_location.x + wing_mirror[i + 1].x - 20 + 250 - angle, boss_location.y + wing_mirror[i + 1].y + 10 + angle + delta_y,
			boss_location.x + wing_mirror[i + 2].x - 10 + 250 - angle, boss_location.y + wing_mirror[i + 2].y + 20 + angle + delta_y,
			boss_location.x + wing_mirror[i + 3].x + 250 - angle, boss_location.y + wing_mirror[i + 3].y + 30 + angle + delta_y, 0xFFFFFF, FALSE, 3.0f);
	}
}

void End::DrawHexagonSphere() const
{
	// ボスの中心座標
	Vector2D center = { boss_location.x + 250 / 2, boss_location.y + 250 / 2 };
	// 六角形の中心
	Vector2D hexa_center;

	// バリアの半径の配列を定義
	float hex_size = 15.0f; // 六角形のサイズ

	// 六角形の間隔（六角形の内接円の半径の2倍）
	float hex_height = (float)sqrt(3) * (float)hex_size / 2; // 六角形の高さ

	//六角形の層の数を管理
	for (int i = 0; i <= 9; ++i) {
		//各層内の六角形の水平方向の位置を管理
		for (int j = -i; j <= i; ++j) {
			//各層内の六角形の垂直方向の位置を管理
			for (int k = -i; k <= i; ++k) {
				// 六角形の中心座標を計算
				//中心の x 座標に対して、水平に 1.5f * hex_size * j だけ移動
				hexa_center.x = center.x + (1.5f * hex_size * j);
				//中心の y 座標に対して、垂直方向に 2.0f * hex_height * k - hex_height * j だけ移動
				hexa_center.y = center.y + (2.0f * hex_height * k - hex_height * j);

				// 半径内にある六角形のみを描画
				float distance = (float)sqrt(pow(hexa_center.x - center.x, 2) + pow(hexa_center.y - center.y, 2));
				if (distance <= 110) {
					// 描画範囲を調整して内部を埋める
					DrawHexagon({ hexa_center.x, hexa_center.y }, hex_size * 0.9f, boss_color); // 0.9fは調整可能
				}
			}
		}
	}
}

void End::DrawHexagon(Vector2D center, float size, int color) const
{
	float angle_space = (float)(2.0f * PI / 6.0f); // 六角形の各頂点の間の角度
	Vector2D vertices[6];

	// 六角形の頂点座標を計算
	//6つの頂点を中心から等距離に配置
	for (int i = 0; i < 6; ++i) {
		float angle = i * angle_space;
		//size は六角形の辺の長さで、cos(angle) は角度に対する水平方向、sin(angle) は角度に対する垂直方向
		vertices[i] = { center.x + size * cos(angle), center.y + size * sin(angle) };
	}

	// 六角形の描画
	for (int i = 0; i < 6; ++i) {
		//(i + 1) % 6 は、最後の頂点から最初の頂点に戻るための処理
		DrawLineAA(vertices[i].x, vertices[i].y + boss_anim, vertices[(i + 1) % 6].x, vertices[(i + 1) % 6].y + boss_anim, color);
	}
}

void End::InvertedWingPositions()
{
	// wing_mirror を更新する
	Vector2D center = { boss_location.x + 250 / 2, boss_location.y + 250 / 2 };

	for (size_t i = 0; i < wing.size(); ++i)
	{
		// wing[i] の X 座標を反転して wing_mirror[i] の X 座標にセット
		wing_mirror[i].x = -wing[i].x; // X座標を反転

		// wing[i] の Y 座標をそのまま wing_mirror[i] の Y 座標にセット
		wing_mirror[i].y = wing[i].y;
	}
}

void End::BossUpdate()
{
	boss_cnt++;

	boss_anim = (float)sin(PI * 2.f / 60.f * up) * 5.f;
	InvertedWingPositions();
	SavePosition();

	boss_location.x -= 2.f * fast_forward;
	boss_location.y += 1.f * fast_forward;

	int c = 0;
	if (boss_cnt <= 60)
	{
		boss_color = RED;
	}
	else if (boss_cnt <=  120 ) {
		boss_color = BLUE;
	}
	else if (boss_cnt <= 180) {
		boss_color = GREEN;
	}
	else if (boss_cnt <= 240) {
		boss_cnt = 0;
	}
}

void End::SavePosition()
{
	std::ofstream outfile("Resource/Dat/BossLocation.txt");
	if (outfile.is_open()) {
		for (int i = 0; i < wing.size(); ++i)
		{
			outfile << wing[i].x << " " << wing[i].y << "\n";
		}
		outfile.close();
	}
}

void End::LoadPosition()
{
	std::ifstream infile("Resource/Dat/BossLocation.txt");
	if (infile.is_open()) {
		for (int i = 0; i < wing.size(); ++i)
		{
			infile >> wing[i].x >> wing[i].y;
		}
		infile.close();
	}
}

void End::EndGameUpdate()
{
	end_anim_count++;
	SetDrawScreen(DX_SCREEN_BACK);
	if (end_anim_count > 90) {
		return;
	}
	if (end_anim_count % 5 == 0)
	{
		ResourceManager::StartSound(swap_se);
	}
	int xnum = (SCREEN_WIDTH / cellSize_) + 1;
	int ynum = (SCREEN_HEIGHT / cellSize_) + 1;
	int eraseNum = ((xnum * ynum) / interval_);
	if (tiles_.size() > eraseNum) {
		tiles_.erase(tiles_.end() - eraseNum, tiles_.end());
	}
	else {
		tiles_.clear();
	}
}