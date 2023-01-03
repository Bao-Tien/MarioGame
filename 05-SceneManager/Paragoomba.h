#pragma once
#include "Enemy.h"

#define ANI_PARAGOOMBA_DIE "ani-red-para-goomba-idle"
#define ANI_PARAGOOMBA_MOVE "ani-red-para-goomba-idle"
#define ANI_PARAGOOMBA_FLAP "ani-red-para-goomba-fly"

#define PARA_BBOX_WIDTH 16*3
#define PARA_BBOX_HEIGHT 53
#define PARA_BBOX_FLY_HEIGHT 60

enum ESTATE_INLEVEL2 {
	MOVE = 0,
	JUMP = 1,
	FLAP = 2,
};

class CParagoomba : public CEnemy
{
protected:
	ESTATE_INLEVEL2 stateInLevel2;
	ULONGLONG newActionInLevel2_start;
	ULONGLONG replayActionInLevel2_start;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CParagoomba(float x, float y) : CEnemy(x, y) {
		type = EEnemy_Type::TROOPAS;
		level = 2;
		stateInLevel2 = ESTATE_INLEVEL2::MOVE;
		OnChangeLevel();
		isAutoChangeDirectionWhenMoveOverRangeX = true;
		newActionInLevel2_start = -1;
		replayActionInLevel2_start = -1;
	}
	void OnChangeLevel() override;
	void ChangeAction();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnCollisionWith();
};