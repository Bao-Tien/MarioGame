#include "Enemy.h"
#include "Mario.h"

CEnemy::CEnemy(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = ENEMY_GRAVITY;
	die_start = -1;
	vx = -ENEMY_MOVE_SPEED;
}

void CEnemy::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CEnemy::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CEnemy*>(e->obj)) return;

	if (e->ny < 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((state == EEnemy_State::DIE))
	{
		if ((GetTickCount64() - die_start > ENEMY_DIE_TIMEOUT)) {
			isDeleted = true;
		}
		return;
	}

	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	UpdateState();
}

void CEnemy::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
}

void CEnemy::SetState(EEnemy_State s) {
	state = s;
}

string CEnemy::GetAnimationFromState() {
	string typeString, stateString;
	if (type == EEnemy_Type::GOOMBA) typeString = "ani-goomba";

	if (state == EEnemy_State::DIE) stateString = "die";
	else if (state == EEnemy_State::MOVE) stateString = "walk";

	return typeString + "-" + stateString;
}
