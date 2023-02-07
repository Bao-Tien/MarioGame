#include "TailMario.h"

void CTailMario::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
}

void CTailMario::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	switch (level)
	{
	case 0:
		SetBoundingBoxSize(0, 0);
		break;
	case 1:
		SetBoundingBoxSize(TAIL_BBOX_WIDTH, TAIL_BBOX_HEIGHT);
		break;
	default:
		break;
	}

	CGameObject::GetBoundingBox(l, t, r, b);
}

string CTailMario::GetAnimationFromState() {
	return "";
}

void CTailMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (level == 1) {
		int a = 0;
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects); // dich chuyen tuc thoi, ko co van toc => ko xai dc SweptAABB
	}
	//DebugOut(L"level: %i - x: %f - y: %f \n", this->level, this->x, this->y);
}