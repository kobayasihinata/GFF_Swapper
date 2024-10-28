#pragma once
#include "../Base/Object.h"

class Tutorial:
	public Object
{
private:

	int num; //何番目のチュートリアルか

public:
	//コンストラクタ
	Tutorial();
	~Tutorial();

	
	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

	//ヒット時の処理
	void Hit(Object* _object)override;

	bool SearchColor(Object* ob) override {
		return false;
	}
};

