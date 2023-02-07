#pragma once
#include "GameObject.h"

#define ID_ANI_COIN "ani-coin"
#define COIN_BBOX_WIDTH 48
#define COIN_BBOX_HEIGHT 48

class CGate : public CGameObject {
	string gateType;
	int gateId;
public:
	CGate(float x, float y, int width, int height, int gateId, string gateType) : CGameObject(x, y) {
		this->BoundingBox_Width = width;
		this->BoundingBox_Height = height;
		this->gateId = gateId;
		this->gateType = gateType;
	}
	void Render();
	int IsBlocking() { return 0; }
	string GetGateType() { return gateType; }
	int GetGateId() { return gateId; }
};
