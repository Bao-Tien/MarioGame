#pragma once
#include "Mushroom.h"

#define ID_ANI_MUSHROOM_GREEN "ani-1-up-mushroom"

class CMushroomGreen : public CMushroom {
public:
	CMushroomGreen(float x, float y) : CMushroom(x, y) {
	}
	string GetAnimationFromState();
};