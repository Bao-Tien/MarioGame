#pragma once
#include "Mario.h"

class CRaccoonMario : public CMario
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CRaccoonMario(float x, float y, int nx, int coin = 0, int point = 0) : CMario(x, y, coin, point) {
		level = EMario_Level::RACCOON;
		this->nx = nx;
	}
};