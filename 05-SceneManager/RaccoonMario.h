#pragma once
#include "Mario.h"
#include "TailMario.h"
#include "PlayScene.h"
#include "TailMario.h"

class CRaccoonMario : public CMario
{
	CTailMario* tailMario;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CRaccoonMario(float x, float y, int nx, int coin = 0, int point = 0) : CMario(x, y, coin, point) {
		level = EMario_Level::RACCOON;
		this->nx = nx;
		tailMario = new CTailMario(x, y);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};