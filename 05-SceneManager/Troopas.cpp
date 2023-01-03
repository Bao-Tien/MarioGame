#include "Troopas.h"
#include "Mario.h"

void CTroopas::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (level) {
		case 0: {
			SetBoundingBoxSize(TROOPAS_BBOX_WIDTH, TROOPAS_BBOX_HEIGHT_DIE);
			break;
		}
		case 1: {
			SetBoundingBoxSize(TROOPAS_BBOX_WIDTH, TROOPAS_BBOX_HEIGHT_DIE);
			break;
		}
		case 2: {
			SetBoundingBoxSize(TROOPAS_BBOX_WIDTH, TROOPAS_BBOX_HEIGHT);
			break;
		}
	}

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CTroopas::OnChangeLevel() {
	switch (level)
	{
		case 0: {
			die_start = GetTickCount64();
			vx = 0;
			vy = 0;
			ay = 0;
			enemyAnimationId = ANI_TROOPAS_DIE;
			break;
		}
		case 1: {
			enemyAnimationId = ANI_TROOPAS_CROUCH;
			crouch_start = GetTickCount64();
			vx = 0;
			break;
		}
		case 2: {
			if (crouch_start) {
				int a = 0;
			}
			enemyAnimationId = ANI_TROOPAS_MOVE;
			vx = nx * ENEMY_MOVE_SPEED;
			break;
		}
		default: {
			break;
		}
	}
}

void CTroopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if ((level == 1))
	{
		if ((GetTickCount64() - crouch_start > ENEMY_CROUCH_TIMEOUT)) {
			level++;
			OnChangeLevel();
		}
		return;
	}

	CEnemy::Update(dt, coObjects);
}
