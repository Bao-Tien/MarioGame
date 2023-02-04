#pragma once
#include "GameObject.h"

#define ENEMY_GRAVITY 0.002f
#define ENEMY_MOVE_SPEED 0.05f
#define ENEMY_DIE_TIMEOUT 500
#define ENEMY_CROUCH_TIMEOUT 50000
#define ENEMY_CHANGESTATE_TIMEOUT 360

enum class EEnemy_Type {
	GOOMBA = 0,
	TROOPAS = 1,
	RED_VENUS = 2,
	FIRE_FLOWER = 3,
	PARAGOOMBA = 4,
	TROOPASGREEN = 5,
	PARATROOPA = 6,
	GREEN_VENUS = 7,
};

class CEnemy : public CGameObject
{
protected:
	float ax;
	float ay;
	float g = ENEMY_GRAVITY;

	ULONGLONG die_start;
	ULONGLONG isAttacked_start;
	EEnemy_Type type;
	string enemyAnimationId;

	int level;
	bool isOnPlatform;
	bool isAttacked;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnChangeLevel() {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


public:
	CEnemy(float x, float y);
	int GetLevel() { return level; }
	void SetIsAttacked(bool x) { this->isAttacked = x; }

	bool attackFromLeft = true;
	bool attackFromTop = false;
	bool attackFromRight = true;
	bool attackFromBottom = true;
};

