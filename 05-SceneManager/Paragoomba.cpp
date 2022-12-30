#include "Paragoomba.h"
#include "Mario.h"

void CParagoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (level) {
	case 0: {
		SetBoundingBoxSize(PARA_BBOX_WIDTH, PARA_BBOX_HEIGHT);
		break;
	}
	case 1: {
		SetBoundingBoxSize(PARA_BBOX_WIDTH, PARA_BBOX_HEIGHT);
		break;
	}
	case 2: {
		SetBoundingBoxSize(PARA_BBOX_WIDTH, PARA_BBOX_FLY_HEIGHT);
		break;
	}
	}

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CParagoomba::OnChangeLevel() {
	DebugOut(L"Troopas level: %d", level);
	switch (level)
	{
	case 0: {
		die_start = GetTickCount64();
		vx = 0;
		vy = 0;
		ay = 0;
		ax = 0;
		enemyAnimationId = ANI_PARAGOOMBA_DIE;
		break;
	}
	case 1: {
		enemyAnimationId = ANI_PARAGOOMBA_MOVE;
		vx = -ENEMY_MOVE_SPEED;
		break;
	}
	case 2: {
		enemyAnimationId = ANI_PARAGOOMBA_FLAP;
		vx = -ENEMY_MOVE_SPEED;
		//newStateInLevel2_start = GetTickCount64();
		//if (stateInLevel2 == ESTATE_INLEVEL2::MOVE) {
		//	ax = -ENEMY_MOVE_SPEED;
		//}
		//else if (stateInLevel2 == ESTATE_INLEVEL2::JUMP) {
		//	//ay = -0.01f;
		//}
		//else if (stateInLevel2 == ESTATE_INLEVEL2::FLAP) {
		//	//ay = -0.05f;
		//}
		break;
	}
	default: {
		break;
	}
	}
}

void CParagoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	/*if ((level == 2))
	{
		if ((GetTickCount64() - newStateInLevel2_start > ENEMY_CHANGESTATE_TIMEOUT)) {
			if (stateInLevel2 == ESTATE_INLEVEL2::MOVE) {
				stateInLevel2 == ESTATE_INLEVEL2::JUMP;
			}
			else if (stateInLevel2 == ESTATE_INLEVEL2::JUMP) {
				stateInLevel2 == ESTATE_INLEVEL2::FLAP;
			}
			else if (stateInLevel2 == ESTATE_INLEVEL2::FLAP) {
				stateInLevel2 == ESTATE_INLEVEL2::MOVE;
			}
			OnChangeLevel();
		}
		return;
	}*/

	CEnemy::Update(dt, coObjects);
}
