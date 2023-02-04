#include "GreenVenus.h"

void CGreenVenus::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	SetBoundingBoxSize(VENUS_GREEN_SHORT_BBOX_WIDTH, VENUS_GREEN_SHORT_BBOX_HEIGHT);
	CGameObject::GetBoundingBox(left, top, right, bottom);
}

string CGreenVenus::GetAnimationFromState() {
	if (isMoveUp) {
		return ANI_VENUS_GREEN_UP;
	}
	return ANI_VENUS_GREEN_DOWN;
}