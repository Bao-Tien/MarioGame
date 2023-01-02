#include "SmallMario.h"

void CSmallMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	SetBoundingBoxSize(MARIO_SMALL_BBOX_WIDTH, MARIO_SMALL_BBOX_HEIGHT);
	CMario::GetBoundingBox(left, top, right, bottom);
}