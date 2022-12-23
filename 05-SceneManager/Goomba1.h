#pragma once
#include "Enemy.h"

#define GOOMBA_BBOX_WIDTH 16*3
#define GOOMBA_BBOX_HEIGHT 14*3
#define GOOMBA_BBOX_HEIGHT_DIE 7*3

class CGoomba1 : public CEnemy
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CGoomba1(float x, float y) : CEnemy(x, y) {
		type = EEnemy_Type::GOOMBA;
	}
	virtual void UpdateState();

};

