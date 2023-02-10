#pragma once
#include "GameObject.h"

#define ID_ANI_BRICKDEBRIS "ani-brick-debris"
#define ID_ANI_BRICKNORMAL "ani-brick"
#define ID_ANI_COIN "ani-coin"
#define BRICK_BBOX_WIDTH 48
#define BRICK_BBOX_HEIGHT 48
#define R_MAX 40

#define BRICKGOLDEN_DIE_TIMEOUT 1500
#define SWITCH_TIMEOUT 9000

class CBrickGolden : public CGameObject {
protected:
	int level;
	int r;
	ULONGLONG die_start;
	ULONGLONG switch_start;
	bool isSWitched;
public:
	CBrickGolden(float x, float y) : CGameObject(x, y) {
		level = 1;
		r = 0;
		die_start = -1;
		switch_start = -1;
		isSWitched = false;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	string GetAnimationFromState();
	void OnOverlapWith(LPCOLLISIONEVENT e);
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnChangeLevel();
	int GetLevel() { return level; }
};