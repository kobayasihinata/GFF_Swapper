#include <math.h>
#include "BoxCollider.h"

bool BoxCollider::HitBox(BoxCollider* bCollider)
{
	bool ret = false;

	//自分の左上座標
	float my_x = location.x;
	float my_y = location.y;
	//自分の中央座標
	float my_cx = my_x + (erea.width / 2);
	float my_cy = my_y + (erea.height / 2);
	//自分の幅と高さの半分
	float my_harf_width = erea.width / 2;
	float my_harf_height = erea.height / 2;

	//相手の左上座標
	float sub_x = bCollider->GetLocation().x;
	float sub_y = bCollider->GetLocation().y;
	//相手の中央座標
	float sub_cx = sub_x + (bCollider->GetErea().width / 2);
	float sub_cy = sub_y + (bCollider->GetErea().height / 2);
	//相手の幅と高さの半分
	float sub_harf_width = bCollider->GetErea().width / 2;
	float sub_harf_height = bCollider->GetErea().height / 2;

	//自分と相手の中心座標の差
	float diff_x = my_cx - sub_cx;
	float diff_y = my_cy - sub_cy;

	//当たり判定の演算
	if (fabsf(diff_x) <= my_harf_width + sub_harf_width &&
		fabsf(diff_y) <= my_harf_height + sub_harf_height)
	{
		ret = true;
	}

	return ret;
}

bool BoxCollider::CheckCollision(Location l, Erea e)
{
	bool ret = false;

	//自分の左上座標
	float my_x = location.x;
	float my_y = location.y;
	//自分の中央座標
	float my_cx = my_x + (erea.width / 2);
	float my_cy = my_y + (erea.height / 2);
	//自分の幅と高さの半分
	float my_harf_width = erea.width / 2;
	float my_harf_height = erea.height / 2;

	//相手の左上座標
	float sub_x = l.x;
	float sub_y = l.y;
	//相手の中央座標
	float sub_cx = sub_x + (e.width / 2);
	float sub_cy = sub_y + (e.height / 2);
	//相手の幅と高さの半分
	float sub_harf_width = e.width / 2;
	float sub_harf_height = e.height / 2;

	//自分と相手の中心座標の差
	float diff_x = my_cx - sub_cx;
	float diff_y = my_cy - sub_cy;

	//当たり判定の演算
	if (fabsf(diff_x) < my_harf_width + sub_harf_width &&
		fabsf(diff_y) < my_harf_height + sub_harf_height)
	{
		ret = true;
	}
	return ret;
}

Location BoxCollider::GetLocation()const
{
	return location;
}
Location BoxCollider::GetCenterLocation()const
{
	Location ret = { 0 };
	ret.x = location.x + erea.width / 2;
	ret.y = location.y + erea.height / 2;
	return ret;
}
Location BoxCollider::GetLocalLocation()const
{
	return local_location;
}
Erea BoxCollider::GetErea()const
{
	return erea;
}
void BoxCollider::SetScreenPosition(Location _world_to_screen, int _impact)
{
	local_location.x = location.x - _world_to_screen.x + _impact;
	local_location.y = location.y - _world_to_screen.y + _impact;
}