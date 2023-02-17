#include "Troopas.h"
#include "Mario.h"
#include "BrickMagic.h"
#include "DeathPlatform.h"
#include "RectPlatform.h"
#include "RectCollision.h"
#include "BrickGolden.h"

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
		case 4: {
			SetBoundingBoxSize(TROOPAS_BBOX_WIDTH, TROOPAS_BBOX_HEIGHT);
			break;
		}
	}

	CEnemy::GetBoundingBox(left, top, right, bottom);
}

void CTroopas::OnChangeLevel() {
	CEnemy::OnChangeLevel();
	switch (level)
	{
		case 0: {
			die_start = GetTickCount64();
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		}
		case 1: {// mai rua attack
			attackFromLeft = true;
			attackFromTop = true;
			attackFromRight = true;
			attackFromBottom = true;
			isAutoChangeDirectionWhenMoveOverRangeX = false;
			isCollidable = 1;
			isBlocking = 1;
			break;
		}
		case 2: { //mai rua dung im
			crouch_start = GetTickCount64();
			attackFromLeft = false;
			attackFromTop = false;
			attackFromRight = false;
			attackFromBottom = false;
			vx = 0;
			isAutoChangeDirectionWhenMoveOverRangeX = false;
			isCollidable = 1;
			isBlocking = 1;
			break;
		}
		case 3: { //di bo
			attackFromLeft = true;
			attackFromTop = false;
			attackFromRight = true;
			attackFromBottom = true;
			vx = nx * ENEMY_MOVE_SPEED;
			break;
		}
		case 4: { // bay nhay
			attackFromLeft = true;
			attackFromTop = false;
			attackFromRight = true;
			attackFromBottom = true;
			vx = nx * ENEMY_MOVE_SPEED * 3;
			vy = -ENEMY_MOVE_SPEED * 16;
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
	if (dynamic_cast<CEnemy*>(e->obj) && level == 1) {
		dynamic_cast<CEnemy*>(e->obj)->ActionIsAttacked();
		return;
	}
	if (dynamic_cast<CDeathPlatform*>(e->obj)) {
		isDeleted = true;
	}
	if (level == 4 && (dynamic_cast<CRectPlatform*>(e->obj) || dynamic_cast<CRectCollision*>(e->obj) 
		|| dynamic_cast<CBrickGolden*>(e->obj))) {
		OnChangeLevel();
		return;
	}
	else if (level == 4 && dynamic_cast<CMario*>(e->obj)) {
		nx = nx * -1;
		OnChangeLevel();
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
	if (level == 2 && isHeld) {
		isCollidable = 0;
		isBlocking = 0;
	}
	else {
		isCollidable = 1;
		isBlocking = 1;
	}
	
	//DebugOut(L"vx: %f, ax: %f, nx: %i\n", vx, ax, nx);
	CEnemy::Update(dt, coObjects);
}

string CTroopas::GetAnimationFromState() {
	switch (level)
	{
	case 0: {
		return ANI_TROOPAS_DIE;
	}
	case 1: {
		return ANI_TROOPAS_CROUCH;
	}
	case 2: {
		return ANI_TROOPAS_CROUCH;
	}
	case 3: {
		return ANI_TROOPAS_MOVE;
	}
	default: {
		break;
	}
	}
}