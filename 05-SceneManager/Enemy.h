#pragma once
#include "GameObject.h"

#define ENEMY_GRAVITY 0.002f
#define ENEMY_MOVE_SPEED 0.05f
#define ENEMY_DIE_TIMEOUT 500

enum class EEnemy_State {
	DIE = 0,
	MOVE = 1,
};

enum class EEnemy_Type {
	GOOMBA = 0,
};

class CEnemy : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	EEnemy_State state;
	EEnemy_Type type;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CEnemy(float x, float y);
	virtual void SetState(EEnemy_State s);
	virtual EEnemy_State GetState() { return state; }
	string GetAnimationFromState();
	virtual void UpdateState() {};
};

