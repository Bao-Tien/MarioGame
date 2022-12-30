#pragma once
#include "Enemy.h"

#define ANI_GOOMBA_DIE "ani-goomba-die"
#define ANI_GOOMBA_MOVE "ani-goomba-walk"

#define GOOMBA_BBOX_WIDTH 48
#define GOOMBA_BBOX_HEIGHT 45
#define GOOMBA_BBOX_HEIGHT_DIE 27

class CGoomba1 : public CEnemy
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CGoomba1(float x, float y) : CEnemy(x, y) {
		type = EEnemy_Type::GOOMBA;
		level = 1;
		OnChangeLevel();
	}
	void OnChangeLevel() override;
	virtual void UpdateState();

};

