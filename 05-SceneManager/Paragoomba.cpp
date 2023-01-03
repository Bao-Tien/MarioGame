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
		vx = nx * ENEMY_MOVE_SPEED;
		break;
	}
	case 2: {
		enemyAnimationId = ANI_PARAGOOMBA_FLAP;
		newActionInLevel2_start = GetTickCount64();
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
		break;
	}
	default: {
		break;
	}
	}
}

void CParagoomba::ChangeAction() {
	replayActionInLevel2_start = GetTickCount64();
	if (stateInLevel2 == ESTATE_INLEVEL2::MOVE) {
			vx = nx * ENEMY_MOVE_SPEED;
			//DebugOut(L"MOVE: %f\n", vy);
		}
		else if (stateInLevel2 == ESTATE_INLEVEL2::JUMP) {
		//ay = -0.01f;
		vx = nx * ENEMY_MOVE_SPEED;
		vy = -ENEMY_MOVE_SPEED * 5;
		//DebugOut(L"JUMP: %f\n", vy);
	}
	else if (stateInLevel2 == ESTATE_INLEVEL2::FLAP) {
		//ay = -0.05f;
		vx = nx * ENEMY_MOVE_SPEED;
		vy = -ENEMY_MOVE_SPEED * 6;
		//DebugOut(L"FLAP: %f\n", vy);
	}
}

void CParagoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if ((level == 2))
	{
		if (GetTickCount64() - newActionInLevel2_start > ENEMY_CHANGESTATE_TIMEOUT) {
			OnChangeLevel();
			ChangeAction();
		}
		else if (vy == 0) {
			ChangeAction();
		}
		/*else {
			if ()
			if (stateInLevel2 == ESTATE_INLEVEL2::FLAP) {
				if (GetTickCount64() - replayActionInLevel2_start >= 100) {
					ChangeAction();
					DebugOut(L"state: %d\n", stateInLevel2);
				}
			}
			else {
				if (GetTickCount64() - replayActionInLevel2_start >= 1000) {
					ChangeAction();
					DebugOut(L"state: %d\n", stateInLevel2);
				}
				else {
					vy = 0;
				}
			}
		}*/
		
	}

	CEnemy::Update(dt, coObjects);
}
