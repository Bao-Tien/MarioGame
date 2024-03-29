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
	CMagicObj::OnCollisionWith(e);
	if (e->obj) {
		if (dynamic_cast<CMario*>(e->obj)) {
			//level = 0;
		}
		else {
			if (e->nx != 0) {
				nx *= -1;
			}
		}
	}
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CMagicObj::Update(dt, coObjects);
	if (isAppeared) {
		vx = nx * MUSHROOM_VX;
		vy += (ay + MUSHROOM_G) * dt;

		if (vy > MUSHROOM_MAX_VY) vy = MUSHROOM_MAX_VY;

		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	else {
		if (yStart - y < MUSHROOM_BBOX_WIDTH) {
			y -= 5;
		}
		else isAppeared = true;
	}
}