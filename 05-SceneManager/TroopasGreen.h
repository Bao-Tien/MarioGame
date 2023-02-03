#pragma once
#include "Troopas.h"

#define ANI_TROOPASGREEN_DIE "ani-green-koopa-troopa-crouch"
#define ANI_TROOPASGREEN_CROUCH "ani-green-koopa-troopa-crouch"
#define ANI_TROOPASGREEN_MOVE "ani-green-koopa-troopa-move"
#define ANI_TROOPASGREEN_FLY "ani-green-koopa-paratroopa-fly"

class CTroopasGreen : public CTroopas
{
protected:
public:	
	CTroopasGreen(float x, float y) : CTroopas(x, y) {
		type = EEnemy_Type::TROOPASGREEN;
		level = 3;

		crouch_start = -1;
		nx = -1;
		flipX = -1;
		isHeld = false;
		justIsHeld = false;
		isAutoChangeDirectionWhenMoveOverRangeX = false;
		OnChangeLevel();
	}
	string GetAnimationFromState();
};