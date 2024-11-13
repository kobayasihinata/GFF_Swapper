#pragma once

#define RED 0xff0000
#define GREEN 0x00ff00
#define BLUE 0x0000ff
#define WHITE 0xffffff

class ColorData
{
public:
	//色情報
//マクロの4色から入れる
	int color = 0;
	int draw_color = 0;		//描画する色
	//色の交換
	void ChangeColor(ColorData* cd);

	void SetColorData(int c)
	{
		color = c;
	}

	int GetColorData()const
	{
		return color;

	}

	void SetDrawColor(int c)
	{
		draw_color = c;
	}

	//色情報を0~3で受け取る
	int GetColorNum(int c)const
	{
		switch (c)
		{
		case RED:
			return 0;
		case GREEN:
			return 1;
		case BLUE:
			return 2;
		case WHITE:
			return 3;
		default:
			return -1;
		}
	}
};

