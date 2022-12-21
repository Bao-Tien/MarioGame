#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"

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

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}

// Enemy
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
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
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

void CMario::GetAnimationFromState() {
	string typeString, stateString;
	if (level == MARIO_LEVEL_SMALL) typeString = ANI_MARIO_LEVEL_SMALL;
	else if (level == MARIO_LEVEL_BIG) typeString = ANI_MARIO_LEVEL_BIG;
	else if (level == MARIO_LEVEL_RACCOON) typeString = ANI_MARIO_LEVEL_RACCOON;
	else typeString = ANI_MARIO_LEVEL_SMALL;

	if (state == MARIO_STATE_IDLE) stateString = ANI_MARIO_STATE_IDLE;
	else if (state == MARIO_STATE_WALK) stateString = ANI_MARIO_STATE_WALK;
	else if (state == MARIO_STATE_RUN) stateString = ANI_MARIO_STATE_RUN;
	else if (state == MARIO_STATE_JUMP) stateString = ANI_MARIO_STATE_WALK_JUMP;
	else if (state == MARIO_STATE_HIGH_JUMP) stateString = ANI_MARIO_STATE_RUN_JUMP;
	else if (state == MARIO_STATE_SIT) stateString = ANI_MARIO_STATE_SIT;
	else if (state == MARIO_STATE_FALL) stateString = ANI_MARIO_STATE_FALL;
	else if (state == MARIO_STATE_SKID) stateString = ANI_MARIO_STATE_SKID;
	else stateString = ANI_MARIO_STATE_IDLE;

	animationId = typeString + "-" + stateString;
}



void CMario::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::KeyboardHandle(int KeyCode, EKeyType type) {
	switch (KeyCode)
	{
	case DIK_1:
		SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_0:
		SetState(MARIO_STATE_DIE);
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
			if (isOnPlatform && level != MARIO_LEVEL_SMALL)
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
				state = MARIO_STATE_IDLE;
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
			SetState(MARIO_STATE_SIT);
		}
		else {
			if (vx == 0)
			{
				SetState(MARIO_STATE_IDLE);
			}
			else
			{
				if ((vx > 0 && ax < 0) || (vx < 0 && ax>0)) {
					SetState(MARIO_STATE_SKID);
				}
				else if (abs(vx) > MARIO_WALKING_SPEED) {
					SetState(MARIO_STATE_RUN);
				}
				else {
					SetState(MARIO_STATE_WALK);
				}
			}
		}
	}
	else {
		if (abs(ay) == MARIO_ACCEL_FLY_X) {
			SetState(MARIO_STATE_FLY);
		}
		else if (abs(vx) > MARIO_WALKING_SPEED)
		{
			SetState(MARIO_STATE_HIGH_JUMP);
		}
		else if (abs(maxVx) <= MARIO_WALKING_SPEED) {
			SetState(MARIO_STATE_JUMP);
		}
		else SetState(MARIO_STATE_FALL);


			
	}
		
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
	}
	level = l;
}

