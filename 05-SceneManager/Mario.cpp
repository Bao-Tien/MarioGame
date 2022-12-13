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

	if (ms < 0 && vx < 0) vx = 0;
	if (ms > 0 && vx > 0) vx = 0;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
	
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

void CMario::SetAnimation() {
	if (state == MARIO_STATE_RUNNING_RIGHT || state == MARIO_STATE_RUNNING_LEFT) {
		switch (level)
		{ 
		case MARIO_LEVEL_SMALL:
			animationId = "ani-small-mario-run";
			break;
		case MARIO_LEVEL_BIG:
			animationId = "ani-big-mario-run";
			break;
		case MARIO_LEVEL_RACCOON:
			animationId = "ani-raccoon-mario-run";
			break;
		default:
			animationId = "ani-small-mario-run";
			break;
		}
	}
	else if (state == MARIO_STATE_WALKING_RIGHT || state == MARIO_STATE_WALKING_LEFT) {
		switch (level)
		{
		case MARIO_LEVEL_SMALL:
			animationId = "ani-small-mario-walk";
			break;
		case MARIO_LEVEL_BIG:
			animationId = "ani-big-mario-walk";
			break;
		case MARIO_LEVEL_RACCOON:
			animationId = "ani-raccoon-mario-walk";
			break;
		default:
			animationId = "ani-small-mario-walk";
			break;
		}
	}
	else if (state == MARIO_STATE_JUMP) {
		switch (level)
		{
		case MARIO_LEVEL_SMALL:
			animationId = "ani-small-mario-jump";
			break;
		case MARIO_LEVEL_BIG:
			animationId = "ani-big-mario-jump";
			break;
		case MARIO_LEVEL_RACCOON:
			animationId = "ani-raccoon-mario-jump";
			break;
		default:
			animationId = "ani-small-mario-jump";
			break;
		}
	}
	else if (state == MARIO_STATE_FLY_RIGHT || state == MARIO_STATE_FLY_LEFT) {
		animationId = "ani-raccoon-mario-fly";
	}
	else if (state == MARIO_STATE_SIT || state == MARIO_STATE_SIT_RELEASE) {
		// not have animation sit
		switch (level)
		{
		case MARIO_LEVEL_SMALL:
			animationId = "ani-small-mario-idle";
			break;
		case MARIO_LEVEL_BIG:
			animationId = "ani-big-mario-idle";
			break;
		case MARIO_LEVEL_RACCOON:
			animationId = "ani-raccoon-mario-idle";
			break;
		default:
			animationId = "ani-small-mario-idle";
			break;
		}
	}
	else if (state == MARIO_STATE_IDLE) {
		switch (level)
		{
		case MARIO_LEVEL_SMALL:
			animationId = "ani-small-mario-idle";
			break;
		case MARIO_LEVEL_BIG:
			animationId = "ani-big-mario-idle";
			break;
		case MARIO_LEVEL_RACCOON:
			animationId = "ani-raccoon-mario-idle";
			break;
		default:
			animationId = "ani-small-mario-idle";
			break;
		}
	}
	else if (state == MARIO_STATE_DIE) {
		switch (level)
		{
		case MARIO_LEVEL_SMALL:
			animationId = "ani-small-mario-die";
			break;
		case MARIO_LEVEL_BIG:
			animationId = "ani-big-mario-die";
			break;
		case MARIO_LEVEL_RACCOON:
			animationId = "ani-raccoon-mario-die";
			break;
		default:
			animationId = "ani-small-mario-die";
			break;
		}
	}
	else animationId = "ani-small-mario-idle";
}

void CMario::Render()
{
	CGameObject::Render();

	RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	ax = 0.0f;
	ay = 0.0f;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				ay = -MARIO_ACCEL_RUN_JUMP_Y;
			else
				ay = -MARIO_ACCEL_JUMP_Y;
		}
		break;
	/*case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;*/
	case MARIO_STATE_FLY_RIGHT:
		if (isSitting) break;
		vy = 0.0f; // de thi no dang roi nhan S tip thi no cap du luc de bay len lai tuc thi, neu ko reset thi no tip tuc roi vi vy dang > 0
		maxVy = -MARIO_FLY_SPEED;
		maxVx = MARIO_FLY_SPEED;
		ax = MARIO_ACCEL_FLY_X;
		ay = -MARIO_ACCEL_FLY_Y;
		nx = 1;
		break;
	case MARIO_STATE_FLY_LEFT:
		if (isSitting) break;
		vy = 0;
		maxVy = -MARIO_FLY_SPEED;
		maxVx = -MARIO_FLY_SPEED;
		ax = -MARIO_ACCEL_FLY_X;
		ay = -MARIO_ACCEL_FLY_X;
		nx = -1;
		break;
	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		//vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	flipX = nx > 0 ? 1 : -1;

	CGameObject::SetState(state);
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

