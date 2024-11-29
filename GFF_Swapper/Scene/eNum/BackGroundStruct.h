#pragma once
#include "../../Utility/Vector2D.h"

//背景アニメーション用
struct BackGroundImage {
    bool flg;
    Vector2D location;
    Vector2D erea;
    bool move_flg;
    Vector2D move_goal;
    float move_speed;
    int color;
    float move_rad;
    int anim_size;
};