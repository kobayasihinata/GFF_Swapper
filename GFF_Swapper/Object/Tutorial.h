#pragma once
#include "Object.h"

class Tutorial:
	public Object
{
private:

	bool is_tutorial; //チュートリアル中か

public:
	//コンストラクタ
	Tutorial();
	~Tutorial();

	void Initialize(Location _location, Erea _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;

	bool SearchColor(Object* ob) override {
		return false;
	}
};

