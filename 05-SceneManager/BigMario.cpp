#include "BigMario.h"

void CBigMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	SetBoundingBoxSize(MARIO_BIG_BBOX_WIDTH, MARIO_BIG_BBOX_HEIGHT);
	CMario::GetBoundingBox(left, top, right, bottom);
}