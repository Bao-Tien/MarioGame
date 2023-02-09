#include "MagicObj.h"
#include "Mario.h"

void CMagicObj::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (level == 0) { return; }
	if (e->obj) {
		if (dynamic_cast<CMario*>(e->obj)) {
			level = 0;
			die_start = GetTickCount64();
		}
	}
}

void CMagicObj::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if ((level == 0))
	{
		if ((GetTickCount64() - die_start > OBJECT_DIE_TIMEOUT)) {
			isDeleted = true;
		}
		return;
	}
	CGameObject::Update(dt, coObjects);
}