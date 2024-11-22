#include <dxlib.h>
#include "Option.h"
#include "GameMain.h"
#include "../Utility/PadInput.h"

Option::Option():
	current_num(0)
{

}

Option::~Option()
{

}

void Option::Initialize()
{

}

void Option::Finalize()
{

}

AbstractScene* Option::Update()
{
	return this;
}


void Option::Draw() const
{
	DrawStringF(0, 0, "option", 0x00ff00, true);
}
