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
		newStateInLevel2_start = GetTickCount64();
		if (stateInLevel2 == ESTATE_INLEVEL2::MOVE) {
			vx = -ENEMY_MOVE_SPEED;
		}
		else if (stateInLevel2 == ESTATE_INLEVEL2::JUMP) {
			//ay = -0.01f;
			vy = -ENEMY_MOVE_SPEED*2;
			vx = -ENEMY_MOVE_SPEED;
		}
		else if (stateInLevel2 == ESTATE_INLEVEL2::FLAP) {
			//ay = -0.05f;
			vx = -ENEMY_MOVE_SPEED*2;
			vy = -ENEMY_MOVE_SPEED;
		}
		break;
	}
	default: {
		break;
	}
	}
}

void CParagoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if ((level == 2))
	{
		DebugOut(L"tg da qua: %d \n", GetTickCount64() - newStateInLevel2_start);
		if ((GetTickCount64() - newStateInLevel2_start > ENEMY_CHANGESTATE_TIMEOUT)) {
			if (stateInLevel2 == ESTATE_INLEVEL2::MOVE) {
				stateInLevel2 = ESTATE_INLEVEL2::JUMP;
				//DebugOut(L"tg chuyen: %d, statecu: MOVE, statemoi: JUMP \n", GetTickCount64() - newStateInLevel2_start);
			}
			else if (stateInLevel2 == ESTATE_INLEVEL2::JUMP) {
				stateInLevel2 = ESTATE_INLEVEL2::FLAP;
				//DebugOut(L"tg chuyen: %d, statecu: JUMP, statemoi: FLAP \n", GetTickCount64() - newStateInLevel2_start);
			}
			else if (stateInLevel2 == ESTATE_INLEVEL2::FLAP) {
				stateInLevel2 = ESTATE_INLEVEL2::MOVE;
				//DebugOut(L"tg chuyen: %d, statecu: FLAP, statemoi: MOVE \n", GetTickCount64() - newStateInLevel2_start);
			}
			OnChangeLevel();
		}
	}

	CEnemy::Update(dt, coObjects);
}
