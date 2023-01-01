#pragma once

#include "GameObject.h"
#include "Animation.h"

#define ID_ANI_COIN "ani-coin"

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 48
#define COIN_BBOX_HEIGHT 48

class CCoin : public CGameObject {
public:
	CCoin(float x, float y) : CGameObject(x, y) {
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	string GetAnimationFromState();
};