#pragma once
#include"BoxCollider.h"

//仮想クラス
class GameMain;

class CharaBase : public BoxCollider
{
protected:
	int image = 0;
	float speed = 0;
	int hp = 0;

public:
	virtual void Update(GameMain* main) = 0;
	virtual void Draw()const = 0;
};