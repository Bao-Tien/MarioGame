#pragma once
#include "GameObject.h"

#define ID_ANI_COIN "ani-coin"
#define COIN_BBOX_WIDTH 48
#define COIN_BBOX_HEIGHT 48

class CCoin : public CGameObject {
	int level;
public:
	CCoin(float x, float y) : CGameObject(x, y) {
		level = 1;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	string GetAnimationFromState();
	void OnCollisionWith(LPCOLLISIONEVENT e);
	int GetLevel() { return level; }
};