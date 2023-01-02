#pragma once
#include "Mario.h"

class CBigMario : public CMario
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CBigMario(float x, float y) : CMario(x, y) {
		this->BoundingBox_Width = MARIO_BIG_BBOX_WIDTH;
		this->BoundingBox_Height = MARIO_BIG_BBOX_HEIGHT;
		level = EMario_Level::BIG;
	}
};

