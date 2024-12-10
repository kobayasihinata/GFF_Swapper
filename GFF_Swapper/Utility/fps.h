#pragma once
#include "DxLib.h"
#include <math.h>
#include "DebugInfomation.h"

class Fps
{
private:
	int start_time;		//測定開始時刻
	int count;			//カウント
	float fps;			//fps
	static const int average = 10;	//平均をカウント数
	static const int x_move = 60;//フレームレート設定
public:
	Fps()
	{
		start_time = 0;
		count = 0;
		fps = 0;
	}
	bool Update()
	{
		//1フレーム目なら時刻を記憶
		if (count == 0)
		{
			start_time = GetNowCount();
		}
		//10フレーム目なら平均を計算する
		if (count == average)
		{
			int t = GetNowCount();
			fps = 1000.f / ((t - start_time) / (float)average);;
			count = 0;
			start_time = t;
		}
		count++;
		return true;
	}
	void Draw()
	{
		//fps描画
		DebugInfomation::Add("fps", fps);
	}
	void Wait()
	{
		//かかった時間
		int took_time = GetNowCount() - start_time;
		//待つべき時間
		int wait_time = count * 1000 / x_move - took_time;
		if (wait_time > 0)
		{
			//待機
			Sleep(wait_time);
		}
	}
};