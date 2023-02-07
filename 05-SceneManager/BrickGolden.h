#pragma once
#include "GameObject.h"

#define ID_ANI_BRICKNORMAL "ani-brick"
#define BRICK_BBOX_WIDTH 48
#define BRICK_BBOX_HEIGHT 48

class CBrickGolden : public CGameObject {
protected:
	int level;
public:
	CBrickGolden(float x, float y) : CGameObject(x, y) {
		level = 1;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	string GetAnimationFromState();
	void OnOverlapWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};