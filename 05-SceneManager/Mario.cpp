#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"
#include "Enemy.h"
#include "DeathPlatform.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (vx > 0) ms = -MARIO_FRICTION;
	else if (vx < 0) ms = MARIO_FRICTION;
	else ms = 0;
	
	// a -> v -> s
	vy += (ay + g) * dt;
	vx += (ax + ms) * dt; // F = ma + m*ms

	// update vx nhung ko lam thay doi huong
	if (vx > 0 && ms > 0) vx = 0;
	if (vx < 0 && ms < 0) vx = 0;

	if (abs(vx) > abs(maxVx)) vx = maxVx;
	if (maxVy != 0 && abs(vy) > abs(maxVy)) vy = maxVy;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	isOnPlatform = false;
	// reset nha nut A
	accelerated = 1;
	// reset nha nut 
	ax = 0;
	ay = 0;

	CCollision::GetInstance()->Process(this, dt, coObjects);

	UpdateState();
	
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

// e->ny < 0 : va cham o duoi chan mario
// e->ny > 0 : va cham tren dau mario
// e->nx != 0 : va cham phuong ngang
void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CEnemy*>(e->obj))
		OnCollisionWithEnemy(e);
	else if (dynamic_cast<CDeathPlatform*>(e->obj))
		OnCollisionWithDeathPlatform(e);
}
//DeathPlatform
void CMario::OnCollisionWithDeathPlatform(LPCOLLISIONEVENT e) {
	SetState(EMario_State::DIE);
	DebugOut(L">>> Mario DIE >>> \n");
}

// Enemy
void CMario::OnCollisionWithEnemy(LPCOLLISIONEVENT e) {
	CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);

	if ((enemy->GetLevel() != 0)) {
		// Conditions when Mario is attacked
		bool condition1 = e->nx < 0 && enemy->attackFromLeft == true;
		bool condition2 = e->ny < 0 && enemy->attackFromTop == true;
		bool condition3 = e->nx > 0 && enemy->attackFromRight == true;
		bool condition4 = e->ny > 0 && enemy->attackFromBottom == true;

		if (condition1 || condition2 || condition3 || condition4) {
			// Mario is attacked
			if (untouchable == 0)
			{
				if (level == EMario_Level::FIRE)
				{
					level = EMario_Level::RACCOON;
					StartUntouchable();
				}
				else if (level == EMario_Level::RACCOON)
				{
					level = EMario_Level::BIG;
					StartUntouchable();
				}
				else if (level == EMario_Level::BIG)
				{
					level = EMario_Level::SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					ay = -MARIO_JUMP_DEFLECT_SPEED;
					ax = 0;
					vx = 0;
					SetState(EMario_State::DIE);
				}
			}
		}
		else {
			// Enemy is attacked (Mario attack enemy)
			ay = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
}

string CMario::GetAnimationFromState() {
	string typeString, stateString;
	if (level == EMario_Level::SMALL) typeString = ANI_MARIO_LEVEL_SMALL;
	else if (level == EMario_Level::BIG) typeString = ANI_MARIO_LEVEL_BIG;
	else if (level == EMario_Level::RACCOON) typeString = ANI_MARIO_LEVEL_RACCOON;
	else typeString = ANI_MARIO_LEVEL_SMALL;

	if (state == EMario_State::IDLE) stateString = ANI_MARIO_STATE_IDLE;
	else if (state == EMario_State::WALK) stateString = ANI_MARIO_STATE_WALK;
	else if (state == EMario_State::RUN) stateString = ANI_MARIO_STATE_RUN;
	else if (state == EMario_State::JUMP) stateString = ANI_MARIO_STATE_WALK_JUMP;
	else if (state == EMario_State::JUMP_HIGH) stateString = ANI_MARIO_STATE_RUN_JUMP;
	else if (state == EMario_State::SIT) stateString = ANI_MARIO_STATE_SIT;
	else if (state == EMario_State::FALL) stateString = ANI_MARIO_STATE_FALL;
	else if (state == EMario_State::SKID) stateString = ANI_MARIO_STATE_SKID;
	else if (state == EMario_State::DIE) stateString = ANI_MARIO_STATE_DIE;
	else stateString = ANI_MARIO_STATE_IDLE;

	return typeString + "-" + stateString;
}



void CMario::Render()
{
	ULONGLONG currentTime = GetTickCount64();
	if (untouchable == 1) {
		// Bat tu
		if (currentTime % 2 == 0) {
			CGameObject::Render();
		}
		
	}
	else {
		// Binh thuong
		CGameObject::Render();
	}
	
	CGameObject::RenderBoundingBox();
	DebugOutTitle(L"MarioState: %d", state);
}

void CMario::KeyboardHandle(int KeyCode, EKeyType type) {
	switch (KeyCode)
	{
	case DIK_1:
		SetLevel(EMario_Level::SMALL);
		break;
	case DIK_2:
		SetLevel(EMario_Level::BIG);
		break;
	case DIK_3:
		SetLevel(EMario_Level::RACCOON);
		break;
	case DIK_0:
		SetState(EMario_State::DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	case DIK_A:
		if (type == EKeyType::KEY_STATE)
		{
			if (isSitting) break;
			accelerated = 2.0f;
		}
		break;
	case DIK_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED * accelerated;
		ax = MARIO_ACCEL_WALK_X * accelerated * 3 / 4;
		nx = 1;
		break;
	case DIK_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED * accelerated;
		ax = -MARIO_ACCEL_WALK_X * accelerated * 3 / 4;
		nx = -1;
		break;
	case DIK_S:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (state == EMario_State::RUN) {
				ay = -MARIO_ACCEL_JUMP_Y * 1.2f;
			}
			else {
				ay = -MARIO_ACCEL_JUMP_Y;
			}
		}
		if (level == EMario_Level::RACCOON && state == EMario_State::RUN) {
			isFly = 1;
		}
		if (level == EMario_Level::RACCOON && isFly == 1) {
			ay = -MARIO_ACCEL_JUMP_Y / 2.0f;
		}
		break;
	case DIK_DOWN:
		if (type == EKeyType::KEY_DOWN) {
			if (isOnPlatform && level != EMario_Level::SMALL)
			{
				isSitting = true;
				vx = 0; vy = 0.0f;
				y += MARIO_SIT_HEIGHT_ADJUST;
			}
			break;
		}
		else if (type == EKeyType::KEY_UP) {
			if (isSitting)
			{
				isSitting = false;
				state = EMario_State::IDLE;
				y = y - MARIO_SIT_HEIGHT_ADJUST - 1.0f;
			}
			break;
		}
	}

	flipX = nx > 0 ? 1 : -1;
}



void CMario::UpdateState() {
	if (state == EMario_State::DIE) return;
	if (isOnPlatform)
	{
		if (isSitting)
		{
			SetState(EMario_State::SIT);
		}
		else {
			if (vx == 0)
			{
				SetState(EMario_State::IDLE);
			}
			else
			{
				if ((vx > 0 && ax < 0) || (vx < 0 && ax>0)) {
					SetState(EMario_State::SKID);
				}
				else if (abs(vx) > MARIO_WALKING_SPEED) {
					SetState(EMario_State::RUN);
				}
				else {
					SetState(EMario_State::WALK);
				}
			}
		}
		isFly = 0;
	}
	else {
		if (abs(ay) == MARIO_ACCEL_FLY_X) {
			SetState(EMario_State::FLY);
		}
		else if (abs(vx) > MARIO_WALKING_SPEED)
		{
			SetState(EMario_State::JUMP_HIGH);
		}
		else if (abs(maxVx) <= MARIO_WALKING_SPEED) {
			SetState(EMario_State::JUMP);
		}
		else SetState(EMario_State::FALL);
	}
}

void CMario::SetState(EMario_State s) {
	state = s;
}

void CMario::SetLevel(EMario_Level l)
{
	// Adjust position to avoid falling off platform
	if (this->level == EMario_Level::SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
	}
	level = l;
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state == EMario_State::DIE) {
		SetBoundingBoxSize(0, 0);
	}
	CGameObject::GetBoundingBox(left, top, right, bottom);
}
