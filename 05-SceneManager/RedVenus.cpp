#include "RedVenus.h"
#include "Mario.h"
#include "FireFlower.h"

void CRedVenus::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	SetBoundingBoxSize(VENUS_RED_BBOX_WIDTH, VENUS_RED_BBOX_HEIGHT);
	CGameObject::GetBoundingBox(left, top, right, bottom);
	
}

void CRedVenus::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isMoveUp && y >= yStart - BoundingBox_Height) {
		y -= 2;
		enemyAnimationId = ANI_VENUS_RED_UP;
		if (y == yStart - BoundingBox_Height) {
			isMoveUp = false;
			CFireFlower* fireFlower = new CFireFlower(x, y);
			playScene->PushToDynamicObjectsFrontMap(fireFlower);
		}
	}

	if (!isMoveUp && y < yStart) {
		y += 2;
		enemyAnimationId = ANI_VENUS_RED_DOWN;
		if (y == yStart) {
			isMoveUp = true;
		}
	}

	CEnemy::Update(dt, coObjects);
}