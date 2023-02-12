#pragma once
#include "GameObject.h"

#define ID_ANI_ENDSCENEITEM "ani-end-scene-world1-1"
#define ENDSCENEITEM_BBOX_WIDTH 48
#define ENDSCENEITEM_BBOX_HEIGHT 48

class CEndSceneItem : public CGameObject {
	int level;
	ULONGLONG die_start;
public:
	CEndSceneItem(float x, float y) : CGameObject(x, y) {
		level = 1;
		die_start = -1;
		isBlocking = 0;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	string GetAnimationFromState();
	void OnCollisionWith(LPCOLLISIONEVENT e);
	int GetLevel() { return level; }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};