#pragma once

#include "GameObject.h"
#include "Animation.h"

#define ID_ANI_BRICK "ani-brick"//10000
#define BRICK_WIDTH 48//16
#define BRICK_BBOX_WIDTH 48//16
#define BRICK_BBOX_HEIGHT 48//16

class CBrick : public CGameObject {
public:
	CBrick(float x, float y) : CGameObject(x, y) {
	}
	void SetAnimation() { animationId = ID_ANI_BRICK; }
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};