#pragma once
#include "GameObject.h"
#include "Animation.h"

#include "debug.h"
#include"define-mario.h"
#include "SampleKeyEventHandler.h"

enum class EMario_State {
	DIE = 0,
	IDLE = 1,
	WALK = 2,
	JUMP = 3,
	JUMP_HIGH = 4,
	FLY = 5,
	RUN = 6,
	SIT = 7,
	FALL = 8,
	SKID = 9,
	HOLD = 10,
	ATTACK = 11,
	KICK = 12,
};

enum class EMario_Level {
	DIE = 0,
	SMALL = 1,
	BIG = 2,
	RACCOON = 3,
	FIRE = 4,
};

class CMario : public CGameObject
{
protected:
	BOOLEAN isSitting;
	float maxVx; 
	float maxVy;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	float ms;
	float g;
	float accelerated;
	float energy;

	EMario_State state;
	EMario_Level level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin; 
	bool canFly;
	LPGAMEOBJECT heldObj;

	void OnCollisionWithEnemy(LPCOLLISIONEVENT e);
	void OnCollisionWithDeathPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithMagicObj(LPCOLLISIONEVENT e);
	
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
		energy = 0;
		accelerated = 1.0f;
		canFly = 0;

		level = EMario_Level::BIG;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
		state = EMario_State::IDLE;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void UpdateState();
	void Render();

	string GetAnimationFromState();

	int IsCollidable()
	{ 
		return (state != EMario_State::DIE); 
	}

	int IsBlocking() { return (state != EMario_State::DIE && untouchable==0); }

	void OnCollisionWith(LPCOLLISIONEVENT e);

	virtual void KeyboardHandle(int KeyCode, EKeyType type);

	void SetLevel(EMario_Level l);
	void SetState(EMario_State s);
	void AddCoin() { this->coin += 1; }
	EMario_Level GetLevel() { return level; }
	EMario_State GetState() { return state; }
	int GetCoin() { return coin; }
	float GetEnergy() { return energy; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};