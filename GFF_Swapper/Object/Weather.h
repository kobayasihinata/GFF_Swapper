#pragma once

#include "Base/Object.h"

class Weather :
    public Object
{
private:
	bool touch_ground;
	Object* delete_object;
public:
	Weather(int _type);//type = 天気(0=通常　1=雨　2=火　3=木)
	~Weather();
	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
	void Draw()const override;
	void Finalize()override;
	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override {
		return false;
	}
};

