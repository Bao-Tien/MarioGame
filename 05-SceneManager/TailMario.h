#pragma once
#include "GameObject.h"
#include "Mario.h"

#define TAIL_BBOX_WIDTH 14*3 + 48
#define TAIL_BBOX_HEIGHT 30

class CTailMario : public CGameObject {
	int level;
public:
	CTailMario(float x, float y) : CGameObject(x, y) {
		level = 0;
	}
	int IsCollidable() { return 1; };
	void Render();
	void OnOverlapWith(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	string GetAnimationFromState();
	void SetLevel(int x) { this->level = x; }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};