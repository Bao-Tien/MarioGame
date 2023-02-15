#pragma once
#pragma once
#include "GameObject.h"

#define ID_ANI_TREE "ani-tree"
#define TREE_BBOX_WIDTH 48
#define TREE_BBOX_HEIGHT 48

class CTree : public CGameObject {
protected:
public:
	CTree(float x, float y) : CGameObject(x, y) {
		isCollidable = 0;
		isBlocking = 0;
	}
	//int IsBlocking() { return 1; }
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	string GetAnimationFromState();
	void OnCollisionWith(LPCOLLISIONEVENT e);
};