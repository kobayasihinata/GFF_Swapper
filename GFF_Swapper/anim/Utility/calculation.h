#pragma once
#define _USE_MATH_DEFINES
#include "Vector2D.h"
#include <math.h>

#define D_R(_d) _d * M_PI / 180
#define R_D(_r) _r * 180 / M_PI


static Vector2D GetUnitVector(Vector2D v) {
	Vector2D tmp;
	float dis;

	dis = sqrtf(powf(v.x, 2) + powf(v.y, 2));
	tmp = v / dis;

	return tmp;
}

static Vector2D GetRotaLocation(Vector2D l, Vector2D base, float ang) {

	//r: 角度(ラジアン)
	//x : 元のX座標
	//y : 元のY座標
	//
	//X = x * cos(r) - y * sin(r)
	//Y = x * sin(r) + y * cos(r)

	Vector2D ret;
	Vector2D tmp = l - base;

	ret.x = tmp.x * cosf(ang) - tmp.y * sinf(ang);
	ret.y = tmp.x * sinf(ang) + tmp.y * cosf(ang);

	ret += base;

	return ret;
}

//ベクトルの長さを計算する
static double GetVectorLength(Vector2D v) {
	return pow((v.x * v.x) + (v.y * v.y), 0.5);
}

//ベクトル内積
static double DotProduct(Vector2D vl, Vector2D vr) {
	return vl.x * vr.x + vl.y * vr.y;
}

//ベクトル外積
static double CrossProduct(Vector2D vl, Vector2D vr) {
	return vl.x * vr.y - vl.y * vr.x;
}

//２つのベクトルABのなす角度θを求める
static double AngleOf2Vector(Vector2D A, Vector2D B)
{
	//　※ベクトルの長さが0だと答えが出ませんので注意してください。

	//ベクトルAとBの長さを計算する
	double length_A = GetVectorLength(A);
	double length_B = GetVectorLength(B);

	//内積とベクトル長さを使ってcosθを求める
	double cos_sita = DotProduct(A, B) / (length_A * length_B);

	//cosθからθを求める
	double sita = acos(cos_sita);

	//ラジアンでなく0～180の角度でほしい場合はコメント外す
	//sita = sita * 180.0 / PI;

	return sita;
}

//当たり判定線上か
static bool GetLineCollision(Vector2D l1, Vector2D l2, Vector2D l, int r) {
	Vector2D unit;
	unit = GetUnitVector(l2 - l1);

	Vector2D tmp = l1;

	for (int i = 0; i < GetVectorLength(l2 - l1); i += r)
	{
		if (GetVectorLength(tmp - l) < r) {
			return true;
		}
		tmp += unit * r;
	}

	return false;
}