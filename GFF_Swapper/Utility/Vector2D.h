#pragma once

class Vector2D
{
public:
	float x;	//x軸
	float y;	//y軸

public:
	Vector2D();						//コンストラクタ
	Vector2D(float scalar);			//コンストラクタ（オーバーロード）
	Vector2D(float mx, float my);	//コンストラクタ（オーバーロード）
	~Vector2D();					//デストラクタ

public:
	//代入オペレーター	
	Vector2D& operator = (const Vector2D& location);

	//加算オペレーター
	const Vector2D operator + (const Vector2D& location)const;
	//加算代入オペレーター
	Vector2D& operator += (const Vector2D& location);

	//減算オペレーター
	const Vector2D operator - (const Vector2D& location)const;
	//減算代入オペレーター
	Vector2D& operator -= (const Vector2D& location);

	//乗算オペレーター(scalar)
	const Vector2D operator * (const float& scalar)const;
	//乗算オペレーター(location)
	const Vector2D operator * (const Vector2D& location)const;
	//乗算代入オペレーター(scalar)
	Vector2D& operator *=(const float& scalar);
	//乗算代入オペレーター(location)
	Vector2D& operator *=(const Vector2D& location);

	//除算オペレーター(scalar)
	const Vector2D operator / (const float& scalar)const;
	//除算オペレーター(location)
	const Vector2D operator / (const Vector2D& location)const;
	//除算代入オペレーター(scalar)
	Vector2D& operator /= (const float& scalar);
	//除算代入オペレーター(location)
	Vector2D& operator /= (const Vector2D& location);

public:
	//座標をInt型にキャストする
	void ToInt(int* x, int* y)const;
};



