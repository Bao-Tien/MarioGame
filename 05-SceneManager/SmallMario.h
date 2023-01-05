#pragma once
#include "Mario.h"

class CSmallMario: public CMario
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CSmallMario(float x, float y, int nx) : CMario(x, y) {
		level = EMario_Level::SMALL;
		this->nx = nx;
	}
};

