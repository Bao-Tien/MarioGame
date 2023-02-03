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
#include "MagicObj.h"
#include "MushroomGreen.h"
#include "Troopas.h"

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
	if (maxVy != 0 && abs(vy) > abs(maxVy)) vy = maxVy * (vy > 0 ? 1 : -1);

	if (energy > 0) {
		energy -= 1;
	}
	if (energy > MAX_ENERGY) energy = MAX_ENERGY;
	if (energy < 0) energy = 0;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	UpdateState();

	isOnPlatform = false;
	
	// reset nha nut 
	ax = 0;
	ay = 0;
	CCollision::GetInstance()->Process(this, dt, coObjects);
	//update Troopas bi nam
	// nha rua
	if (accelerated == 1 && heldObj != NULL) {
		((CTroopas*)heldObj)->SetIsHeld(false);
		SetState(EMario_State::KICK);
		//da rua
		if (nx > 0) {
			((CTroopas*)heldObj)->SetSpeedX(ENEMY_MOVE_SPEED * 5);
		}
		else {
			((CTroopas*)heldObj)->SetSpeedX(-ENEMY_MOVE_SPEED * 5);
		}
		heldObj = NULL;
	}
	//lay vi tri theo Mario
	if (heldObj != NULL) {
		float xTroopas, yTroopas;
		if (nx > 0) {
			xTroopas = x + BoundingBox_Width / 2 + heldObj->GetBoundingBoxSize().x / 2;
		}
		else {
			xTroopas = x - BoundingBox_Width / 2 - heldObj->GetBoundingBoxSize().x / 2;
		}
		yTroopas = y;
		heldObj->SetPosition(xTroopas, yTroopas);
	}
	// reset nha nut A
	accelerated = 1;
	//canFly = false;
	//DebugOut(L"state: %i", state);
	DebugOutTitle(L"untouchable: %i, state: %i, level: %i, x: %f, y: %f, vx: %f, vy:%f", 
		untouchable, state, level, x, y, vx, vy);
	//DebugOut(L"canFly: %i \n", canFly);
}

// e->ny < 0 : va cham o duoi chan mario
// e->ny > 0 : va cham tren dau mario
// e->nx != 0 : va cham phuong ngang
void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMagicObj*>(e->obj)) {
		if (dynamic_cast<CMushroomGreen*>(e->obj)) {
			// ko tang level
		}
		else {
			OnCollisionWithMagicObj(e);
		}
	}

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CEnemy*>(e->obj) && state != EMario_State::ATTACK) {
		OnCollisionWithEnemy(e);
	}
	else if (dynamic_cast<CDeathPlatform*>(e->obj)) {
		OnCollisionWithDeathPlatform(e);
	}
	else if (dynamic_cast<CCoin*>(e->obj) && dynamic_cast<CCoin*>(e->obj)->GetLevel() != 0) {
		AddCoin();
		//DebugOut(L"coin %i\n : ", coin);
	}
}

// MagicObj
void CMario::OnCollisionWithMagicObj(LPCOLLISIONEVENT e) {
	if (dynamic_cast<CMagicObj*>(e->obj)->GetLevel() == 0) {
		return;
	}
	AddPoint(1000);
	if (level == EMario_Level::SMALL) {
		SetLevel(EMario_Level::BIG);
		return;
	}
	else if (level == EMario_Level::BIG) {
		SetLevel(EMario_Level::RACCOON);
		return;
	}
	else if (level == EMario_Level::RACCOON) {
	}
	
}

// DeathPlatform
void CMario::OnCollisionWithDeathPlatform(LPCOLLISIONEVENT e) {
	SetState(EMario_State::DIE);
	DebugOut(L">>> Mario DIE >>> \n");
}

// Enemy
void CMario::OnCollisionWithEnemy(LPCOLLISIONEVENT e) {

	CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);

	if (enemy != NULL && enemy->GetLevel() != 0) {
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
				}
				else if (level == EMario_Level::RACCOON)
				{
					level = EMario_Level::BIG;
				}
				else if (level == EMario_Level::BIG)
				{
					level = EMario_Level::SMALL;
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
			//cam rua
			if (dynamic_cast<CTroopas*>(e->obj) &&
				dynamic_cast<CTroopas*>(e->obj)->GetLevel() == 2) {
				if (accelerated != 2.0f) {
					//da rua
					SetState(EMario_State::KICK);
					if (nx > 0) {
						dynamic_cast<CTroopas*>(e->obj)->SetSpeedX(ENEMY_MOVE_SPEED * 5);
					}
					else {
						dynamic_cast<CTroopas*>(e->obj)->SetSpeedX(-ENEMY_MOVE_SPEED * 5);
					}
				}
				else {
					// cam rua
					heldObj = dynamic_cast<CTroopas*>(e->obj);
					SetState(EMario_State::HOLD);
					dynamic_cast<CTroopas*>(e->obj)->SetIsHeld(true);
				}
			}
			else {
				// Enemy is attacked (Mario attack enemy)
				ay = -MARIO_JUMP_DEFLECT_SPEED;
				vy = 0;
				AddPoint(100);
			}
		}
	}
}

string CMario::GetAnimationFromState() {
	string typeString, stateString;
	if (state == EMario_State::DIE && level != EMario_Level::SMALL) return "";

	if (level == EMario_Level::SMALL) typeString = ANI_MARIO_LEVEL_SMALL;
	else if (level == EMario_Level::BIG) typeString = ANI_MARIO_LEVEL_BIG;
	else if (level == EMario_Level::RACCOON) typeString = ANI_MARIO_LEVEL_RACCOON;
	else if (level == EMario_Level::FIRE) typeString = ANI_MARIO_LEVEL_FIRE;
	else typeString = ANI_MARIO_LEVEL_SMALL;

	if (state == EMario_State::IDLE) stateString = ANI_MARIO_STATE_IDLE;
	else if (state == EMario_State::WALK || (state == EMario_State::RUN && energy < MAX_ENERGY)) stateString = ANI_MARIO_STATE_WALK;
	else if (state == EMario_State::RUN && energy == MAX_ENERGY) stateString = ANI_MARIO_STATE_RUN;
	else if (state == EMario_State::JUMP) stateString = ANI_MARIO_STATE_WALK_JUMP;
	else if (state == EMario_State::JUMP_HIGH) stateString = ANI_MARIO_STATE_RUN_JUMP;
	else if (state == EMario_State::SIT) stateString = ANI_MARIO_STATE_SIT;
	else if (state == EMario_State::HOLD) stateString = ANI_MARIO_STATE_HOLD;
	else if (state == EMario_State::KICK) stateString = ANI_MARIO_STATE_KICK;
	else if (state == EMario_State::FALL) stateString = ANI_MARIO_STATE_FALL;
	else if (state == EMario_State::SKID) stateString = ANI_MARIO_STATE_SKID;
	else if (state == EMario_State::ATTACK) {
		stateString = ANI_MARIO_STATE_ATTACK;
	}
	else if (state == EMario_State::FLY) stateString = ANI_MARIO_STATE_FLY;
	else if (state == EMario_State::DIE) stateString = ANI_MARIO_STATE_DIE;
	else stateString = ANI_MARIO_STATE_IDLE;

	return typeString + "-" + stateString;
}

void CMario::Render()
{
	flipX = nx > 0 ? 1 : -1;
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
	//DebugOutTitle(L"MarioState: %d", state);
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
		// nhan A => quay duoi
		if (type == EKeyType::KEY_DOWN) {
			if (level == EMario_Level::RACCOON || level == EMario_Level::FIRE) {
				attack_start = GetTickCount64();
				SetState(EMario_State::ATTACK);
				DebugOut(L"attack: %i \n", state);
			}
		}
		// de A => chay
		if (type == EKeyType::KEY_STATE)
		{
			if (isSitting) break;
			//tang toc
			accelerated = 2.0f;
			if (state != EMario_State::ATTACK) {
				energy += 3;
			}
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
		if (type == EKeyType::KEY_STATE) {
			
		}
		if (type == EKeyType::KEY_DOWN) {
			if (isOnPlatform && state != EMario_State::JUMP && state != EMario_State::JUMP_HIGH)
			{
				if (state == EMario_State::RUN) {
					ay = -MARIO_ACCEL_JUMP_Y * 1.2f;
				}
				else {
					ay = -MARIO_ACCEL_JUMP_Y;
				}
			}

			if (level == EMario_Level::RACCOON && canFly == true) {
				ay = -MARIO_GRAVITY;
				vy = -0.5f;
				//DebugOut(L" Mario bay >>> \n");
			}

			if (level == EMario_Level::RACCOON && state == EMario_State::FALL) {
				ay = -MARIO_GRAVITY;
				vy = 0.05f;
				//DebugOut(L" Mario bay cham xuong >>> \n");
			}
		}
		break;
	case DIK_DOWN:
		if (type == EKeyType::KEY_DOWN) {
			if (isOnPlatform && level != EMario_Level::SMALL)
			{
				isSitting = true;
				vx = 0; vy = 0.0f;
				y -= MARIO_SIT_HEIGHT_ADJUST;
			}
			break;
		}
		else if (type == EKeyType::KEY_UP) {
			if (isSitting)
			{
				isSitting = false;
				SetState(EMario_State::IDLE);
				y -= MARIO_SIT_HEIGHT_ADJUST;
			}
			break;
		}
	}
}


void CMario::UpdateState() {
	if (state == EMario_State::DIE) return;
	if (state == EMario_State::ATTACK) {
		if (GetTickCount64() - attack_start < TIME_ATTACK_RACCON_MARIO) {
			return; // con tg attack thi ko chuyen state
		}
	}
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
				canFly = false;
			}
			else
			{
				if ((vx > 0 && ax < 0) || (vx < 0 && ax>0)) {
					energy = -10;
					SetState(EMario_State::SKID);
				}
				else if (abs(vx) > MARIO_WALKING_SPEED) {
					SetState(EMario_State::RUN);
					if (energy == MAX_ENERGY && level == EMario_Level::RACCOON) {
						canFly = true;
					}
				}
				else {
					SetState(EMario_State::WALK);
				}
			}
		}
	}
	else {
		if (vy < 0) {
			if (canFly) {
				SetState(EMario_State::FLY);
			}
			else {
				if (abs(vx) > MARIO_WALKING_SPEED)
				{
					SetState(EMario_State::JUMP_HIGH);
				}
				else if (abs(maxVx) <= MARIO_WALKING_SPEED) {
					SetState(EMario_State::JUMP);
				}
			}
		}
		else {
				if (canFly) {
					SetState(EMario_State::FALL);
				}
			}
	}
}

bool CMario::CheckChangeState(EMario_State newState) {
	if (state == EMario_State::DIE) {
		return false;
	}
	if (newState == EMario_State::DIE) {
		return true;
	}
	if (state == EMario_State::IDLE) {
		if (newState == EMario_State::WALK || newState == EMario_State::RUN
			|| newState == EMario_State::JUMP || newState == EMario_State::JUMP_HIGH
			|| newState == EMario_State::SIT || newState == EMario_State::KICK
			|| newState == EMario_State::IDLE || newState == EMario_State::ATTACK)
		{
			return true;
		}
	}
	if (state == EMario_State::WALK) {
		if (newState == EMario_State::IDLE || newState == EMario_State::RUN
			|| newState == EMario_State::SKID || newState == EMario_State::JUMP
			|| newState == EMario_State::JUMP_HIGH || newState == EMario_State::KICK
			|| newState == EMario_State::SIT || newState == EMario_State::WALK
			|| newState == EMario_State::ATTACK)
		{
			return true;
		}
	}
	if (state == EMario_State::RUN) {
		if (newState == EMario_State::WALK || newState == EMario_State::HOLD
			|| newState == EMario_State::SKID || newState == EMario_State::JUMP
			|| newState == EMario_State::JUMP_HIGH || newState == EMario_State::KICK
			|| newState == EMario_State::SIT || newState == EMario_State::FLY
			|| newState == EMario_State::RUN || newState == EMario_State::ATTACK);
		{
			return true;
		}
	}
	if (state == EMario_State::JUMP) {
		if (newState == EMario_State::IDLE || newState == EMario_State::ATTACK)
		{
			return true;
		}
	}
	if (state == EMario_State::JUMP_HIGH) {
		if (newState == EMario_State::IDLE || newState == EMario_State::ATTACK)
		{
			return true;
		}
	}
	if (state == EMario_State::FLY) {
		if (newState == EMario_State::FALL || newState == EMario_State::FLY)
		{
			return true;
		}
	}
	if (state == EMario_State::FALL) {
		if (newState == EMario_State::IDLE || newState == EMario_State::FALL
			|| newState == EMario_State::ATTACK)
		{
			return true;
		}
	}
	if (state == EMario_State::SIT) {
		if (newState == EMario_State::IDLE)
		{
			return true;
		}
	}
	if (state == EMario_State::SKID) {
		if (newState == EMario_State::IDLE || newState == EMario_State::WALK
			|| newState == EMario_State::RUN || newState == EMario_State::SKID
			|| newState == EMario_State::JUMP_HIGH || newState == EMario_State::JUMP
			|| newState == EMario_State::ATTACK)
		{
			return true;
		}
	}
	if (state == EMario_State::HOLD) {
		if (newState == EMario_State::HOLD || newState == EMario_State::WALK
			|| newState == EMario_State::RUN || newState == EMario_State::SKID
			|| newState == EMario_State::JUMP_HIGH || newState == EMario_State::JUMP
			|| newState == EMario_State::IDLE)
		{
			return true;
		}
	}
	if (state == EMario_State::KICK) {
		if (newState == EMario_State::IDLE || newState == EMario_State::WALK
			|| newState == EMario_State::RUN || newState == EMario_State::SIT
			|| newState == EMario_State::JUMP_HIGH || newState == EMario_State::JUMP
			|| newState == EMario_State::ATTACK)
		{
			return true;
		}
	}
	if (state == EMario_State::ATTACK) {
		if (newState == EMario_State::RUN || newState == EMario_State::WALK
			|| newState == EMario_State::IDLE || newState == EMario_State::SIT
			|| newState == EMario_State::JUMP_HIGH || newState == EMario_State::JUMP)
		{
			return true;
		}
	}
}

void CMario::SetState(EMario_State newState) {
	if (CheckChangeState(newState)) {
		state = newState;
	}
}

void CMario::SetLevel(EMario_Level l)
{
	// Adjust position to avoid falling off platform
	if (this->level == EMario_Level::SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
		if (nx > 0) {
			x = x - 2;
		}
		else {
			x = x + 2;
		}
	}
	level = l;
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (state == EMario_State::DIE) {
		SetBoundingBoxSize(0, 0);
	}
	if (state == EMario_State::SIT) {
		SetBoundingBoxSize(MARIO_BIG_SITTING_BBOX_WIDTH, MARIO_BIG_SITTING_BBOX_HEIGHT);
	}
	CGameObject::GetBoundingBox(left, top, right, bottom);
}
