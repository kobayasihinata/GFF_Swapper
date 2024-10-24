#include"Weather.h"
#include "../Scene/GameMain.h"
#include "Stage.h"
#include "../Utility/common.h"

Weather::Weather(int _type):touch_ground(false), delete_object(0)
{
	type = _type;
	can_swap = FALSE;
	can_hit = TRUE;
}

Weather::~Weather()
{

}

void Weather::Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)
{
	location = _location;
	erea = _erea;
	color = _color_data;
	object_pos = _object_pos;
}

void Weather::Update(GameMain* _g)
{
	switch (type)
	{
	case FIRE:
		location.y += 7;
		if (touch_ground == true || local_location.y > SCREEN_HEIGHT)
		{
			_g->DeleteObject(object_pos, this);
			//木に当たったなら木は消え、その場所に火を作る
			if (delete_object != 0 && delete_object->GetObjectType() == WOOD)
			{
				if (delete_object->GetCanSwap() == TRUE)
				{
					_g->CreateObject(new Stage(3), delete_object->GetLocation(), delete_object->GetErea(), 3);
				}
				else
				{
					_g->CreateObject(new Stage(6), delete_object->GetLocation(), delete_object->GetErea(), 6);
				}
				_g->DeleteObject(delete_object->GetObjectPos(),this);
			}
		}
		break;
	case WATER:
		location.y += 20;
		if (touch_ground == true || local_location.y > SCREEN_HEIGHT)
		{
			_g->DeleteObject(object_pos, this);
			//火に当たったなら火は消え、その場所に水を作る
			if (delete_object != 0 && delete_object->GetObjectType() == FIRE)
			{
				if (delete_object->GetCanSwap() == TRUE)
				{
					_g->CreateObject(new Stage(5), delete_object->GetLocation(), delete_object->GetErea(), 5);
				}
				else
				{
					_g->CreateObject(new Stage(8), delete_object->GetLocation(), delete_object->GetErea(), 8);
				}
				_g->DeleteObject(delete_object->GetObjectPos(), this);
			}
		}
		break;
	case WOOD:
		location.y += 20;
		if (touch_ground == true || local_location.y > SCREEN_HEIGHT)
		{
			_g->DeleteObject(object_pos, this);
			//水に当たったなら水は消え、その場所に草を作る
			if (delete_object != 0 && delete_object->GetObjectType() == WATER)
			{
				if (delete_object->GetCanSwap() == TRUE)
				{
					_g->CreateObject(new Stage(4), delete_object->GetLocation(), delete_object->GetErea(), 4);
				}
				else
				{
					_g->CreateObject(new Stage(7), delete_object->GetLocation(), delete_object->GetErea(), 7);
				}
				_g->DeleteObject(delete_object->GetObjectPos(), this);
			}
		}
		break;
	default:
		break;
	}
}

void Weather::Draw()const
{
	DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, color, TRUE);
}

void Weather::Finalize()
{

}

void Weather::Hit(Object* _object)
{
	touch_ground = true;
	delete_object = _object;
}