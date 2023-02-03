#pragma once
#include "Enemy.h"
#include "PlayScene.h"

#define ANI_VENUS_RED_UP "ani-red-venus-fire-trap-headup"
#define ANI_VENUS_RED_DOWN "ani-red-venus-fire-trap-headdown"
#define VENUS_RED_BBOX_WIDTH 48
#define VENUS_RED_BBOX_HEIGHT 48*2

class CRedVenus : public CEnemy
{
protected:
	CPlayScene* playScene;
	float yStart;
	bool isMoveUp; // co dang di chuyen len ko
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CRedVenus(float x, float y, CPlayScene* playScene) : CEnemy(x, y) {
		type = EEnemy_Type::RED_VENUS;
		yStart = y;
		isMoveUp = 1;
		level = 1;
		g = 0;
		attackFromTop = true;
		this->playScene = playScene;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	string GetAnimationFromState();
};