#pragma once
#include "GameObject.h"
#include "Mario.h"

#define TAIL_BBOX_WIDTH 14*3//60
#define TAIL_BBOX_HEIGHT 30

//class CTailMario : public CMario {
//	int level;
//	bool beingAttack = false;
//public:
//	CTailMario(float x, float y) : CMario(x, y) {
//		level = 1;
//		beingAttack = false;
//	}
//	void Render();
//	/*int IsBlocking() { return 1; }
//	int IsCollidable() { return 1; };*/
//	void GetBoundingBox(float& l, float& t, float& r, float& b);
//	string GetAnimationFromState();
//	void SetBeingAttack(bool x) { this->beingAttack = x; }
//	bool GetBeingAttack() { return this->beingAttack; }
//};
class CTailMario : public CGameObject {
	int level;
	bool beingAttack = false;
public:
	CTailMario(float x, float y) : CGameObject(x, y) {
		level = 1;
		beingAttack = false;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	string GetAnimationFromState();
	void SetBeingAttack(bool x) { this->beingAttack = x; }
	bool GetBeingAttack() { return this->beingAttack; }
};