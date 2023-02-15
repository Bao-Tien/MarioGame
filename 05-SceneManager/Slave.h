#pragma once
#include "GameObject.h"

#define SLAVE_MOVE_SPEED 0.05f
#define SLAVE_ANI "ani-slave"
#define RANGE_X_RADIUS 25

class CSlave : public CGameObject
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CSlave(float x, float y) : CGameObject(x, y) {
		nx = 1;
		vx = SLAVE_MOVE_SPEED;
		isAutoChangeDirectionWhenMoveOverRangeX = true;
		moveRangeX = D3DXVECTOR2(x - RANGE_X_RADIUS, x + RANGE_X_RADIUS);
		isCollidable = 0;
		isBlocking = 0;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	string GetAnimationFromState();
};

