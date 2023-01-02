#pragma once

#include "GameObject.h"

#define ID_ANI_BRICKQUESTION "ani-question-block"
#define ID_ANI_BRICKQUESTION_EMPTY "ani-empty-block"
#define BRICKQUESTION_BBOX_WIDTH 48
#define BRICKQUESTION_BBOX_HEIGHT 48

class CBrickQuestion : public CGameObject {
	int level;
public:
	CBrickQuestion(float x, float y) : CGameObject(x, y) {
		level = 1;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlocking() { return 0; }
	string GetAnimationFromState();
	void OnCollisionWith(LPCOLLISIONEVENT e);
	//void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};