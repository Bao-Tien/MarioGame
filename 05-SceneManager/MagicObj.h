#pragma once
#include "GameObject.h"
class CMagicObj : public CGameObject
{
protected:
	int level;
	ULONGLONG die_start;
public:
	CMagicObj(float x, float y) : CGameObject(x, y) {
		die_start = -1;
		isCollidable = 1;
		isBlocking = 0;
	}
	int GetLevel() { return level; }
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

