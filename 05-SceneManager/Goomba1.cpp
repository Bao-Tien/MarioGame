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

	CEnemy::GetBoundingBox(left, top, right, bottom);
}

void CGoomba1::OnChangeLevel() {
	switch (level)
	{
		case 0: {
			die_start = GetTickCount64();
			vx = 0;
			vy = 0;
			ay = 0;
			break;
		}
		case 1: {
			vx = nx * ENEMY_MOVE_SPEED;
			break;
		}
		default: {
			break;
		}
	}
}
string CGoomba1::GetAnimationFromState() {
	switch (level)
	{
		case 0: {
			return ANI_GOOMBA_DIE;
		}
		case 1: {
			return ANI_GOOMBA_MOVE;
		}
		default: {
			break;
		}
	}
}