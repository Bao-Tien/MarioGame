#include "TroopasGreen.h"

string CTroopasGreen::GetAnimationFromState() {
	switch (level)
	{
	case 0: {
		return ANI_TROOPASGREEN_DIE;
	}
	case 1: {
		return ANI_TROOPASGREEN_CROUCH;
	}
	case 2: {
		return ANI_TROOPASGREEN_CROUCH;
	}
	case 3: {
		return ANI_TROOPASGREEN_MOVE;
	}
	case 4: {
		return ANI_TROOPASGREEN_FLY;
	}
	default: {
		break;
	}
	}
}