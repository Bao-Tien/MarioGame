#pragma once
#include "Mario.h"

class CBigMario : public CMario
{
public:
	CBigMario(float x, float y) : CMario(x, y) {
		this->BoundingBox_Width = MARIO_BIG_BBOX_WIDTH;
		this->BoundingBox_Height = MARIO_BIG_BBOX_HEIGHT;
	}
};

