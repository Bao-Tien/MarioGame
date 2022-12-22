#pragma once
#include "Mario.h"

class CSmallMario: public CMario
{
public:
	CSmallMario(float x, float y) : CMario(x, y) {
		this->BoundingBox_Width = MARIO_SMALL_BBOX_WIDTH;
		this->BoundingBox_Height = MARIO_SMALL_BBOX_HEIGHT;
	}
};

