#include "FireFlower.h"
#include "Mario.h"

#define FIRE_BULLET_DIE_TIMEOUT 10000

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
	CEnemy::OnChangeLevel();
	switch (level)
	{
		case 0: {
			break;
		}
		case 1: {
			die_start = GetTickCount64();
			break;
		}
		default: {
			break;
		}
	}
}

void CFireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if ((GetTickCount64() - die_start > FIRE_BULLET_DIE_TIMEOUT)) {
		isDeleted = true;
		return;
	}
	
	CEnemy::Update(dt, coObjects);
}

string CFireFlower::GetAnimationFromState() {
	switch (level)
	{
		case 0: {
			return "";
		}
		case 1: {
			return ID_ANI_FIREFLOWER;
		}
		default: {
			break;
		}
	}
}