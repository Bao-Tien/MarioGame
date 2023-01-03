#pragma once
#include "Mario.h"

class CRaccoonMario : public CMario
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CRaccoonMario(float x, float y, int nx) : CMario(x, y) {
		this->BoundingBox_Width = MARIO_SMALL_BBOX_WIDTH;
		this->BoundingBox_Height = MARIO_SMALL_BBOX_HEIGHT;
		level = EMario_Level::RACCOON;
		this->nx = nx;
	}
};