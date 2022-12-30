#pragma once
#include "Mario.h"

class CSmallMario: public CMario
{
	//virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CSmallMario(float x, float y) : CMario(x, y) {
		this->BoundingBox_Width = MARIO_SMALL_BBOX_WIDTH;
		this->BoundingBox_Height = MARIO_SMALL_BBOX_HEIGHT;
	}
};

