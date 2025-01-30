#include "ColorData.h"

void ColorData::ChangeColor(ColorData* cd)
{
    int tmp;    //色交換用

    tmp = color;
    color = cd->GetColorData();
    cd->SetColorData(tmp);

}

void ColorData::SetColorData(int c)
{
    this->color = c;
}

int ColorData::GetColorData()const
{
    return color;
}

//色情報を0~3で受け取る
int ColorData::GetColorNum(int c)const
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