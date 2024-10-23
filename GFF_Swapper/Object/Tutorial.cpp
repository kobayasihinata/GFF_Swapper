#include "Tutorial.h"
#include "../Scene/GameMain.h"

Tutorial::Tutorial() :is_tutorial(false)
{
}

Tutorial::~Tutorial()
{
}

void Tutorial::Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)
{
	location = _location;
	erea = _erea;

	color = _color_data;

	object_pos = _object_pos;

	is_tutorial = false;
}

void Tutorial::Update(GameMain* _g)
{
}

void Tutorial::Draw() const
{
	DrawBoxAA(local_location.x, local_location.y - 550, local_location.x + erea.x, local_location.y + erea.y, GetColor(255, 255, 255), FALSE);
}

void Tutorial::Finalize()
{
}

