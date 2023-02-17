#include "RedVenus.h"
#include "Mario.h"
#include "FireFlower.h"

void CRedVenus::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	SetBoundingBoxSize(VENUS_RED_BBOX_WIDTH, VENUS_RED_BBOX_HEIGHT);
	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CRedVenus::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isMoveUp && y >= yStart - BoundingBox_Height) {
		y -= VENUS_MOVE_D;
		if (y == yStart - BoundingBox_Height) {
			isMoveUp = false;
			CFireFlower* fireFlower = new CFireFlower(x, y - 30);
			playScene->PushToDynamicObjectsFrontMap(fireFlower);
		}
	}

	if (!isMoveUp && y < yStart) {
		y += VENUS_MOVE_D;
		if (y == yStart) {
			isMoveUp = true;
		}
	}

	CEnemy::Update(dt, coObjects);
}

string CRedVenus::GetAnimationFromState() {
	if (isMoveUp) {
		return ANI_VENUS_RED_UP;
	}
	return ANI_VENUS_RED_DOWN;
}