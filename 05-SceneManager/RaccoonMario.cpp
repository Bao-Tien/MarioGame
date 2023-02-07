#include "RaccoonMario.h"

void CRaccoonMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	SetBoundingBoxSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);
	CMario::GetBoundingBox(left, top, right, bottom);
}
void CRaccoonMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CMario::Update(dt, coObjects);
	
	if (this->state == EMario_State::ATTACK) {
		tailMario->SetLevel(1);
	}
	else {
		tailMario->SetLevel(0);
	}
	tailMario->SetPosition(x, y + 10); // dich chuyen tuc thoi
	tailMario->Update(dt, coObjects);
}

void CRaccoonMario::Render() {
	CMario::Render();
	tailMario->Render();
}