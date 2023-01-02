#include "RedVenus.h"
#include "Mario.h"

void CRedVenus::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (level) {
	case 0: {
		SetBoundingBoxSize(VENUS_RED_BBOX_WIDTH, VENUS_RED_BBOX_HEIGHT);
		break;
	}
	case 1: {
		SetBoundingBoxSize(VENUS_RED_BBOX_WIDTH, VENUS_RED_BBOX_HEIGHT);
		break;
	}
	}

	CGameObject::GetBoundingBox(left, top, right, bottom);
	
}

void CRedVenus::OnChangeLevel() {
	switch (level)
	{
		case 0: {
			die_start = GetTickCount64();
			y = yStart;
			break;
		}
		case 1: {
			break;
		}
		default: {
			break;
		}
	}
}

void CRedVenus::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (level == 1) {
		if (isMoveUp && y > yStart - BoundingBox_Height) {
			y -= 3;
			enemyAnimationId = ANI_VENUS_RED_UP;
			if (y == yStart - BoundingBox_Height) {
				isMoveUp = false;
			}
		}

		if (!isMoveUp && y <= yStart) {
			y += 3;
			enemyAnimationId = ANI_VENUS_RED_DOWN;
			if (y == yStart) {
				isMoveUp = true;
			}
		}
	}

	CEnemy::Update(dt, coObjects);
}