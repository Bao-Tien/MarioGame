#include "BrickQuestion.h"
#include "Mario.h"

void CBrickQuestion::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
}

void CBrickQuestion::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	SetBoundingBoxSize(BRICKQUESTION_BBOX_WIDTH, BRICKQUESTION_BBOX_HEIGHT);
	CGameObject::GetBoundingBox(l, t, r, b);
}

string CBrickQuestion::GetAnimationFromState() {
	switch (level)
	{
	case 0:
		return ID_ANI_BRICKQUESTION_EMPTY;
	case 1:
		return ID_ANI_BRICKQUESTION;
	default:
		break;
	}
}

void CBrickQuestion::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (!e->obj->IsBlocking()) return;

	if (e->ny < 0) {
		if (dynamic_cast<CMario*>(e->obj)) {
			y -= 2;
			level = 0;
		}
	}
}

//void CBrickQuestion::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {
//	CGameObject::Update(dt, coObjects);
//	CCollision::GetInstance()->Process(this, dt, coObjects);
//}