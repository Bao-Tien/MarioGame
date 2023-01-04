#include "Mushroom.h"
#include "debug.h"
#include "Mario.h"

void CMushroom::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	switch (level)
	{
	case 0:
		SetBoundingBoxSize(0, 0);
		break;
	case 1:
		SetBoundingBoxSize(MUSHROOM_BBOX_WIDTH, MUSHROOM_BBOX_HEIGHT);
		break;
	default:
		break;
	}

	CGameObject::GetBoundingBox(l, t, r, b);
}

string CMushroom::GetAnimationFromState() {
	switch (level)
	{
	case 0:
		return "";
	case 1:
		return ID_ANI_MUSHROOM;
	default:
		break;
	}
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (e->obj) {
		if (dynamic_cast<CMario*>(e->obj)) {
			level = 0;
		}
	}
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);
	if (isAppeared) {
		vx = MUSHROOM_VX;
		vy += (ay + MUSHROOM_G) * dt;

		if (vy > MUSHROOM_MAX_VY) vy = MUSHROOM_MAX_VY;

		CCollision::GetInstance()->Process(this, dt, NULL);
	}
	else {
		if (yStart - y < 48) {
			y -= 3;
		}
		else isAppeared = true;
	}
	

	//Reset
	ay = 0;
}