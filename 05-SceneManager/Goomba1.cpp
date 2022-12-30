#include "Goomba1.h"
#include "Mario.h"

void CGoomba1::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (level) {
		case 0: {
			SetBoundingBoxSize(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT_DIE);
			break;
		}
		case 1: {
			SetBoundingBoxSize(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
			break;
		}
	}

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CGoomba1::OnChangeLevel() {
	switch (level)
	{
		case 0: {
			die_start = GetTickCount64();
			vx = 0;
			vy = 0;
			ay = 0;
			enemyAnimationId = ANI_GOOMBA_DIE;
			break;
		}
		case 1: {
			enemyAnimationId = ANI_GOOMBA_MOVE;
			vx = -ENEMY_MOVE_SPEED;
			break;
		}
		default: {
			break;
		}
	}
}

void CGoomba1::UpdateState() {
	if (vx != 0) {
		SetState(EEnemy_State::MOVE);
	}
	else SetState(EEnemy_State::DIE);
}