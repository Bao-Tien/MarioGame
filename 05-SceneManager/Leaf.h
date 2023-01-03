#pragma once
#include "GameObject.h"

#define ID_ANI_LEAF "ani-super-leaf-red"
#define LEAF_BBOX_WIDTH 48
#define LEAF_BBOX_HEIGHT 48

#define LEAF_G 0.002f
#define LEAF_MAX_VY 0.05f
#define LEAF_VX 0.08f
#define RANGE_X_RADIUS 64 // px

class CLeaf : public CGameObject {
	int level;
	float ay;
public:
	CLeaf(float x, float y) : CGameObject(x, y) {
		level = 1;
		vx = LEAF_VX;
		ay = -0.0045f;
		isAutoChangeDirectionWhenMoveOverRangeX = true;
		moveRangeX = D3DXVECTOR2(x - RANGE_X_RADIUS, x + RANGE_X_RADIUS);
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	string GetAnimationFromState();
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};