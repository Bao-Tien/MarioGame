#pragma once
#include "Mushroom.h"

#define ID_ANI_SWITCH_BLOCK "ani-switch-block-blue"
#define ID_ANI_SWITCH_BLOCK_DIE "ani-switch-block-blue-die"

class CSwitchBlock : public CMushroom {
public:
	CSwitchBlock(float x, float y) : CMushroom(x, y) {
	}
	int IsBlocking() { return 1; }
	string GetAnimationFromState();
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};
