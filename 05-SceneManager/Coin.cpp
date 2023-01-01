#include "Coin.h"
#include "Animations.h"

void CCoin::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	SetBoundingBoxSize(COIN_BBOX_WIDTH, COIN_BBOX_HEIGHT);
	CGameObject::GetBoundingBox(l, t, r, b);
}

string CCoin::GetAnimationFromState() {
	return ID_ANI_COIN;
}