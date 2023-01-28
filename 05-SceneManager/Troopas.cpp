#include "Troopas.h"
#include "Mario.h"
#include "BrickMagic.h"

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
		SetBoundingBoxSize(TROOPAS_BBOX_WIDTH, TROOPAS_BBOX_HEIGHT_DIE);
		break;
	}
	case 3: {
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
		attackFromLeft = true;
		attackFromTop = true;
		attackFromRight = true;
		attackFromBottom = true;
		isAutoChangeDirectionWhenMoveOverRangeX = false;
		break;
	}
	case 2: {
		enemyAnimationId = ANI_TROOPAS_CROUCH;
		crouch_start = GetTickCount64();
		attackFromLeft = false;
		attackFromTop = false;
		attackFromRight = false;
		attackFromBottom = false;
		vx = 0;
		isAutoChangeDirectionWhenMoveOverRangeX = false;
		break;
	}
	case 3: {
		enemyAnimationId = ANI_TROOPAS_MOVE;
		attackFromLeft = true;
		attackFromTop = false;
		attackFromRight = true;
		attackFromBottom = true;
		vx = nx * ENEMY_MOVE_SPEED;
		break;
	}
	default: {
		break;
	}
	}
}

void CTroopas::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (isHeld == true) {
		return;
	}

	CEnemy::OnCollisionWith(e);
}

void CTroopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (level == 2)
	{
		if ((GetTickCount64() - crouch_start > ENEMY_CROUCH_TIMEOUT)) {
			level++;
			OnChangeLevel();
		}
	}
	if (nx != 0) {
		vx = abs(vx) * nx;
	}
	
	//DebugOut(L"vx: %f, ax: %f, nx: %i\n", vx, ax, nx);
	CEnemy::Update(dt, coObjects);
}
