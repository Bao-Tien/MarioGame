#pragma once
#include "RedVenus.h"

#define ANI_VENUS_GREEN_UP "ani-green-venus-fire-trap-short-headup"
#define ANI_VENUS_GREEN_DOWN "ani-green-venus-fire-trap-short-headdown"
#define VENUS_GREEN_SHORT_BBOX_WIDTH 48
#define VENUS_GREEN_SHORT_BBOX_HEIGHT 72

class CGreenVenus : public CRedVenus
{
protected:
public:
	CGreenVenus(float x, float y, CPlayScene* playScene) : CRedVenus(x, y, playScene) {
		type = EEnemy_Type::GREEN_VENUS;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	string GetAnimationFromState();
};
