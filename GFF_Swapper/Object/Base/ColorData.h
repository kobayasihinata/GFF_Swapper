#pragma once
#include "../../Utility/common.h"

class ColorData
{
public:

	int color = 0;			//色情報  マクロの4色から入れる

public:
	//色の交換
	void ChangeColor(ColorData* cd);

	//色を変更する
	void SetColorData(int c);

	//色情報を取得
	int GetColorData()const;

	//色情報を0~3で受け取る
	int GetColorNum(int c)const;
};

