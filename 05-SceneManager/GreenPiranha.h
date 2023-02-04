#pragma once
#include "RedVenus.h"

#define ANI_GREEN_PIRANHA "ani-green-piranha-plant-attack"
#define GREEN_PIRANHA_BBOX_WIDTH 48
#define GREEN_PIRANHA_SHORT_BBOX_HEIGHT 72

class CGreenPiranha : public CRedVenus
{
protected:
public:
	CGreenPiranha(float x, float y, CPlayScene* playScene) : CRedVenus(x, y, playScene) {
		type = EEnemy_Type::GREEN_VENUS;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	string GetAnimationFromState();
};
