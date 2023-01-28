#include "Coin.h"
#include "Animations.h"
#include "Mario.h"

void CCoin::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	switch (level)
	{
	case 0:
		SetBoundingBoxSize(0, 0);
		break;
	case 1:
		SetBoundingBoxSize(COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT);
		break;
	default:
		break;
	}
	
	CGameObject::GetBoundingBox(l, t, r, b);
}

string CCoin::GetAnimationFromState() {
	switch (level)
	{
	case 0:
		return "";
	case 1:
		return ID_ANI_COIN;
	default:
		break;
	}
}

void CCoin::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (e->obj) {
		level = 0;
	}
}