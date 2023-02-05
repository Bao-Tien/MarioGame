#include "RaccoonMario.h"

void CRaccoonMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	SetBoundingBoxSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);
	CMario::GetBoundingBox(left, top, right, bottom);
}
void CRaccoonMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	tailMario->SetPosition(x, y + 10);
	if (this->state == EMario_State::ATTACK) {
		tailMario->SetBeingAttack(true);
	}
	else tailMario->SetBeingAttack(false);
	CMario::Update(dt, coObjects);
}