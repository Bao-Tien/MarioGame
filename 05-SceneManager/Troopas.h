
#pragma once
#include "Enemy.h"

#define ANI_TROOPAS_DIE "ani-red-koopa-troopa-crouch"
#define ANI_TROOPAS_CROUCH "ani-red-koopa-troopa-crouch"
#define ANI_TROOPAS_MOVE "ani-red-koopa-troopa-move"

#define TROOPAS_BBOX_WIDTH 16*3
#define TROOPAS_BBOX_HEIGHT 26*3
#define TROOPAS_BBOX_HEIGHT_DIE 16*3

class CTroopas : public CEnemy
{
protected:
	ULONGLONG crouch_start;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CTroopas(float x, float y) : CEnemy(x, y) {
		type = EEnemy_Type::TROOPAS;
		level = 2;
		OnChangeLevel();
		crouch_start = -1;
		nx = -1;
		flipX = -1;
		isAutoChangeDirectionWhenMoveOverRangeX = true;
	}
	void OnChangeLevel() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

