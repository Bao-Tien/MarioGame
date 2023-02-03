#pragma once
#include "TroopasGreen.h"

class CParaTroopa : public CTroopasGreen
{
protected:
public:
	CParaTroopa(float x, float y) : CTroopasGreen(x, y) {
		type = EEnemy_Type::PARAGOOMBA;
		level = 4;

		crouch_start = -1;
		nx = -1;
		flipX = -1;
		isHeld = false;
		justIsHeld = false;
		isAutoChangeDirectionWhenMoveOverRangeX = false;
		OnChangeLevel();
	}
};