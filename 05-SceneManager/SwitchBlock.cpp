#include "Mario.h"
#include "SwitchBlock.h"

string CSwitchBlock::GetAnimationFromState() {
	switch (level)
	{
	case 0:
		return ID_ANI_SWITCH_BLOCK_DIE;
	case 1:
		return ID_ANI_SWITCH_BLOCK;
	default:
		break;
	}
}

void CSwitchBlock::OnCollisionWith(LPCOLLISIONEVENT e) {
	if(level == 0) return;
	if (e->obj) {
		if (dynamic_cast<CMario*>(e->obj) && e->ny !=0) {
			level = 0;
			this->y = this->y + 27;
		}
	}
}

void CSwitchBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isAppeared == false) {
		if (yStart - y < MUSHROOM_BBOX_WIDTH) {
			y -= 5;
		}
		else isAppeared = true;
	}
}