#pragma once
#include "Mario.h"

class CBigMario : public CMario
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CBigMario(float x, float y, int nx) : CMario(x, y) {
		level = EMario_Level::BIG;
		this->nx = nx;
	}
};

