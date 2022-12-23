#include "Goomba1.h"
#include "Mario.h"

void CGoomba1::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (state) {
		case EEnemy_State::DIE: {
			SetBoundingBoxSize(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT_DIE);
			break;
		}
		case EEnemy_State::MOVE: {
			SetBoundingBoxSize(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
			break;
		}
	}

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CGoomba1::OnCollisionWith(LPCOLLISIONEVENT e) {
	CEnemy::OnCollisionWith(e);
	if (e->ny > 0) {
		if (dynamic_cast<CMario*>(e->obj)) {
			die_start = GetTickCount64();
			vx = 0;
			vy = 0;
			ay = 0;
		}
	}
}

void CGoomba1::UpdateState() {
	if (vx != 0) {
		SetState(EEnemy_State::MOVE);
	}
	else SetState(EEnemy_State::DIE);
}