#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"
#include "RectCollision.h"
#include "Enemy.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (vx > 0) ms = -MARIO_FRICTION;
	else if (vx < 0) ms = MARIO_FRICTION;
	else ms = 0;

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

	CCollision::GetInstance()->Process(this, dt, coObjects);
	UpdateState();
	// reset nha nut A
	accelerated = 1;
	// reset nha nut Left , Right
	ax = 0;
	ay = 0;
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
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}

// RectCollision
void CMario::OnCollisionWithRectCollision(LPCOLLISIONEVENT e) {
	CRectCollision* rectCollision = dynamic_cast<CRectCollision*>(e->obj);

	if (e->ny != 0) {

	}
}

// Enemy
void CMario::OnCollisionWithEnemy(LPCOLLISIONEVENT e) {
	CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (enemy->GetState() != EEnemy_State::DIE)
		{
			//enemy->SetState(EEnemy_State::DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (enemy->GetState() != EEnemy_State::DIE)
			{
				if (level > EMario_Level::SMALL)
				{
					level = EMario_Level::SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(EMario_State::DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > EMario_Level::SMALL)
				{
					level = EMario_Level::SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(EMario_State::DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
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
	else stateString = ANI_MARIO_STATE_IDLE;

	return typeString + "-" + stateString;
}



void CMario::Render()
{
	CGameObject::Render();
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
			if (abs(this->vx) > MARIO_WALKING_SPEED) {
				ay = -MARIO_ACCEL_JUMP_Y * 1.2f;
			}
			else {
				ay = -MARIO_ACCEL_JUMP_Y;
			}
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

