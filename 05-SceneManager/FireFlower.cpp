#include "FireFlower.h"
#include "Mario.h"

void CFireFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (level) {
	case 0: {
		SetBoundingBoxSize(0, 0);
		break;
	}
	case 1: {
		SetBoundingBoxSize(FIREFLOWER_BBOX_WIDTH, FIREFLOWER_BBOX_HEIGHT);
		break;
	}
	}

	CGameObject::GetBoundingBox(left, top, right, bottom);

}

void CFireFlower::OnChangeLevel() {
	switch (level)
	{
		case 0: {
			enemyAnimationId = "";
			break;
		}
		case 1: {
			enemyAnimationId = ID_ANI_FIREFLOWER;
			break;
		}
		default: {
			break;
		}
	}
}

void CFireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CEnemy::Update(dt, coObjects);
}