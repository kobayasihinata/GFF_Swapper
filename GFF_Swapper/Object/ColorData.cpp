#include "ColorData.h"

void ColorData::ChangeColor(ColorData* cd)
{
    int tmp;

    tmp = color;
    color = cd->GetColorData();
    draw_color = cd->GetColorData();
    cd->SetColorData(tmp);
    cd->SetDrawColor(tmp);

}
