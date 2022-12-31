#pragma once

#pragma once
#include "Enemy.h"

#define ANI_VENUS_RED_UP "ani-red-venus-fire-trap-headup"
#define ANI_VENUS_RED_DOWN "ani-red-venus-fire-trap-headdown"
#define VENUS_RED_BBOX_WIDTH 48
#define VENUS_RED_BBOX_HEIGHT 48*2

class CRedVenus : public CEnemy
{
protected:
	float yStart;
	bool isMoveUp; // co dang di chuyen len ko
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CRedVenus(float x, float y) : CEnemy(x, y) {
		type = EEnemy_Type::RED_VENUS;
		level = 1;
		OnChangeLevel();
		yStart = y;
		y = y - 3;
		isMoveUp = 1;
		g = 0;
		vx = 0;
		vy = 0;
		ax = 0;
		ay = 0;
	}
	void OnChangeLevel() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};