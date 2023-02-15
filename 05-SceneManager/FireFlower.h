#pragma once
#include "Enemy.h"

#define ID_ANI_FIREFLOWER "ani-fire-bullet"
#define FIREFLOWER_BBOX_WIDTH 30
#define FIREFLOWER_BBOX_HEIGHT 30

#define FIREFLOWER_VY 0.02f
#define FIREFLOWER_VX 0.04f

class CFireFlower : public CEnemy
{
protected:
	CPlayScene* playScene;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CFireFlower(float x, float y) : CEnemy(x, y) {
		type = EEnemy_Type::FIRE_FLOWER;
		level = 1;
		OnChangeLevel();
		attackFromTop = true;
		attackFromLeft = true;
		attackFromRight = true;
		attackFromBottom = true;
		g = 0;
		vx = -FIREFLOWER_VX;
		vy = FIREFLOWER_VY;
	}
	int IsCollidable() { return 0; };
	int IsBlocking() { return 0; }
	void OnChangeLevel() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	string GetAnimationFromState();
};