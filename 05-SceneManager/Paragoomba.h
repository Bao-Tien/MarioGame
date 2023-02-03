#pragma once
#include "Enemy.h"

#define ANI_PARAGOOMBA_DIE "ani-red-goomba-die"
#define ANI_PARAGOOMBA_BEINGATTACKED "ani-red-goomba-walk"
#define ANI_PARAGOOMBA_MOVE "ani-red-para-goomba-idle"
#define ANI_PARAGOOMBA_FLAP "ani-red-para-goomba-fly"


#define PARA_BBOX_WIDTH 48
#define PARA_BBOX_HEIGHT 53
#define PARA_BBOX_FLY_HEIGHT 60

#define GOOMBA_BBOX_WIDTH 48
#define GOOMBA_BBOX_HEIGHT 45
#define GOOMBA_BBOX_HEIGHT_DIE 27

#define TIMELIVE 3
#define TOTAL_STEPS 7

class CParagoomba : public CEnemy
{
protected:
	int T = TIMELIVE; //chu ki
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int indexAction;
public:
	CParagoomba(float x, float y) : CEnemy(x, y) {
		type = EEnemy_Type::PARAGOOMBA;
		level = 2;
		OnChangeLevel();
		indexAction = 0;
	}
	void OnChangeLevel() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	string GetAnimationFromState();
};