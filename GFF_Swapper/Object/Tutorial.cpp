#include "Tutorial.h"
#include "../Scene/GameMain.h"

Tutorial::Tutorial(): tutorial_num(0) , tutorial_time(0)
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

	type = TUTORIAL;

	//is_tutorial = false;
}

void Tutorial::Update(GameMain* _g)
{
}

void Tutorial::Draw() const
{
	//DrawBoxAA(local_location.x, local_location.y - 550, local_location.x + erea.x, local_location.y + erea.y, GetColor(255, 255, 255), FALSE);

	/*if (is_tutorial) {
		DrawFormatString(0, 400, 0xFFFFFF, "TUTORIAL",TRUE);
	}*/
	
	//DrawBoxAA(local_location.x, local_location.y - 500, local_location.x + erea.x, local_location.y + erea.y, GetColor(255, 255, 255), FALSE);

	if (tutorial_num == 1)
	{
		DrawTutorial();
	}

}

void Tutorial::Finalize()
{
}

void Tutorial::Hit(Object* _object)
{
	/*if (_object->GetObjectType() == PLAYER)
	{
		is_tutorial = true;
	}*/

	if (_object->GetObjectType() == PLAYER)
	{
		tutorial_num = 1;
	}
}

void Tutorial::DrawTutorial() const
{
	DrawBoxAA(local_location.x / 2, local_location.y - 500, local_location.x + erea.x + 200, local_location.y + erea.y, GetColor(0, 0, 0), TRUE);
}

