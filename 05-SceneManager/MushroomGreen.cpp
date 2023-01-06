#include "MushroomGreen.h"

string CMushroomGreen::GetAnimationFromState() {
	switch (level)
	{
	case 0:
		return "";
	case 1:
		return ID_ANI_MUSHROOM_GREEN;
	default:
		break;
	}
}