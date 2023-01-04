#pragma once
#include "GameObject.h"

#define ID_ANI_MUSHROOM "ani-super-mushroom"
#define MUSHROOM_BBOX_WIDTH 48
#define MUSHROOM_BBOX_HEIGHT 48

#define MUSHROOM_G 0.002f
#define MUSHROOM_MAX_VY 0.05f
#define MUSHROOM_VX 0.08f

class CMushroom : public CGameObject {
	int level;
	float ay;
	bool isAppeared = false;
	float yStart;
public:
	CMushroom(float x, float y) : CGameObject(x, y) {
		level = 1;
		yStart = y;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	string GetAnimationFromState();
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};