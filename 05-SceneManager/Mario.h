#pragma once
#include "GameObject.h"
#include "Animation.h"

#include "debug.h"
#include"define-mario.h"
#include "SampleKeyEventHandler.h"

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx; 
	float maxVy;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	float ms;
	float g;
	float accelerated;

	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin; 

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		maxVy = 0.0f;
		ax = 0.0f;
		ay = 0.0f; 
		g = MARIO_GRAVITY;
		ms = 0;
		accelerated = 1.0f;

		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void UpdateState();
	void Render();
	//void SetState(int state);
	void GetAnimationFromState();

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void KeyboardHandle(int KeyCode, EKeyType type);

	void SetLevel(int l);
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};