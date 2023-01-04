#pragma once
#include "MagicObj.h"

#define ID_ANI_MUSHROOM "ani-super-mushroom"
#define MUSHROOM_BBOX_WIDTH 48
#define MUSHROOM_BBOX_HEIGHT 48

#define MUSHROOM_G 0.002f
#define MUSHROOM_MAX_VY 0.3f
#define MUSHROOM_VX 0.07f

class CMushroom : public CMagicObj {
	int level;
	float ay;
	bool isAppeared = false;
	float yStart;
public:
	CMushroom(float x, float y) : CMagicObj(x, y) {
		level = 1;
		yStart = y;
		nx = 1;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
	string GetAnimationFromState();
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};