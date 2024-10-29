#pragma once
#include "../Base/Object.h"

class Tutorial:
	public Object
{
private:

	int tutorial_num; //何番目のチュートリアルか
	int tutorial_time;

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

	void DrawTutorial()const;
};

