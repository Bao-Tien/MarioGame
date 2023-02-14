#include "Slave.h"

void CSlave::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	SetBoundingBoxSize(48, 48);
	CGameObject::GetBoundingBox(left, top, right, bottom);
}

string CSlave::GetAnimationFromState() {
	return SLAVE_ANI;
}

void CSlave::Render()
{
	CGameObject::Render();
	CGameObject::RenderBoundingBox();
}

void CSlave::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	flipX = nx > 0 ? 1 : -1;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}