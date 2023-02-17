#pragma once
#include "GameObject.h"

#define ID_ANI_COIN "ani-coin"
#define COIN_BBOX_WIDTH 48
#define COIN_BBOX_HEIGHT 48

class CGateSelectionScene : public CGameObject {
	int id;
public:
	CGateSelectionScene(float x, float y, int gateId) : CGameObject(x, y) {
		this->id = gateId;
	}
	int IsBlocking() { return 0; }
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	string GetAnimationFromState();
	int GetId() { return this->id; }
};
