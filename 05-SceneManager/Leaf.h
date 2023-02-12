#pragma once
#include "MagicObj.h"

#define ID_ANI_LEAF "ani-super-leaf-red"
#define LEAF_BBOX_WIDTH 48
#define LEAF_BBOX_HEIGHT 48

#define LEAF_G 0.002f
#define LEAF_MAX_VY 0.05f
#define LEAF_VX 0.09f
#define LEAF_AX 0.008f
#define RANGE_X_RADIUS 64 // px

class CLeaf : public CMagicObj {
	float ay;
public:
	CLeaf(float x, float y) : CMagicObj(x, y) {
		level = 1;
		vx = LEAF_VX;
		ay = -LEAF_AX;
		isAutoChangeDirectionWhenMoveOverRangeX = true;
		moveRangeX = D3DXVECTOR2(x - RANGE_X_RADIUS, x + RANGE_X_RADIUS);
		isCollidable = 0;
		isBlocking = 1;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	string GetAnimationFromState();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};