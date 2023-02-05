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
		SetBoundingBoxSize(TAIL_BBOX_WIDTH + 50, TAIL_BBOX_HEIGHT);
		break;
	default:
		break;
	}

	CGameObject::GetBoundingBox(l, t, r, b);
}

string CTailMario::GetAnimationFromState() {
	return "";
}