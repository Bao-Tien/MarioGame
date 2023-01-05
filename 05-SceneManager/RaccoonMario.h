#pragma once
#include "Mario.h"

class CRaccoonMario : public CMario
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CRaccoonMario(float x, float y, int nx) : CMario(x, y) {
		level = EMario_Level::RACCOON;
		this->nx = nx;
	}
};