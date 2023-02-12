#include "FireMario.h"

void CFireMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	SetBoundingBoxSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);
	CMario::GetBoundingBox(left, top, right, bottom);
}

void CFireMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CMario::Update(dt, coObjects);

	if (this->state == EMario_State::ATTACK) {
		fireBullet = new CFireBullet(x, y, nx);
	}
	if (fireBullet) {
		fireBullet->Update(dt, coObjects);
	}
}

void CFireMario::Render() {
	CMario::Render();
	if (fireBullet) {
		fireBullet->Render();
	}
}