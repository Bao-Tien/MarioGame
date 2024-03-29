#pragma once
#include "GameObject.h"
#include "Animation.h"

#include "debug.h"
#include"define-mario.h"

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
	int point;
	bool canFly;
	ULONGLONG attack_start;
	LPGAMEOBJECT heldObj;
	bool isWin;

	void OnCollisionWithEnemy(LPCOLLISIONEVENT e);
	void OnCollisionWithDeathPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithMagicObj(LPCOLLISIONEVENT e);
	
public:
	CMario(float x, float y, int coin = 0, int point = 0) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		maxVy = 0.85f;
		ax = 0.0f;
		ay = 0.0f;
		g = MARIO_GRAVITY;
		ms = 0;
		energy = 0;
		accelerated = 1.0f;
		canFly = 0;
		attack_start = -1;

		level = EMario_Level::BIG;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		this->coin = coin;
		this->point = point;
		state = EMario_State::IDLE;
		isCollidable = 1;
		isBlocking = 1;
		isWin = false;
	}
	bool IsWin() { return this->isWin; }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void UpdateState();
	void Render();

	string GetAnimationFromState();

	int IsCollidable()
	{ 
		return (state != EMario_State::DIE); 
	}

	int IsBlocking() { return (state != EMario_State::DIE); }

	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnOverlapWith(LPCOLLISIONEVENT e) override;

	void KeyboardHandle(int KeyCode, EKeyType type);

	void SetLevel(EMario_Level l);
	bool CheckChangeState(EMario_State newState);
	void SetState(EMario_State newState);
	void AddCoin() { this->coin += 1; }
	void AddPoint(int p) { this->point += p; }
	EMario_Level GetLevel() { return level; }
	EMario_State GetState() { return state; }
	int GetCoin() { return coin; }
	int GetPoint() { return point; }
	float GetEnergy() { return energy; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};