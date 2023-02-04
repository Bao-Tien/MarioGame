#include "GreenPiranha.h"

void CGreenPiranha::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	SetBoundingBoxSize(GREEN_PIRANHA_BBOX_WIDTH, GREEN_PIRANHA_SHORT_BBOX_HEIGHT);
	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CGreenPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isMoveUp && y >= yStart - BoundingBox_Height) {
		y -= 2;
		if (y == yStart - BoundingBox_Height) {
			isMoveUp = false;
		}
	}

	if (!isMoveUp && y < yStart) {
		y += 2;
		if (y == yStart) {
			isMoveUp = true;
		}
	}

	CEnemy::Update(dt, coObjects);
}

string CGreenPiranha::GetAnimationFromState() {
	return ANI_GREEN_PIRANHA;
}