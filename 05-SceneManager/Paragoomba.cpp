#include "Paragoomba.h"
#include "Mario.h"
#include <iostream>

void CParagoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (level) {
		case 0: {
			SetBoundingBoxSize(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT_DIE);
			break;
		}
		case 1: {
			SetBoundingBoxSize(GOOMBA_BBOX_WIDTH, GOOMBA_BBOX_HEIGHT);
			break;
		}
		case 2: {
			SetBoundingBoxSize(PARA_BBOX_WIDTH, PARA_BBOX_HEIGHT);
			break;
		}
		case 3: {
			SetBoundingBoxSize(PARA_BBOX_WIDTH, PARA_BBOX_FLY_HEIGHT);
			break;
		}
		case 4: {
			SetBoundingBoxSize(PARA_BBOX_WIDTH, PARA_BBOX_FLY_HEIGHT);
			break;
		}
	}
	//SetBoundingBoxSize(PARA_BBOX_WIDTH, PARA_BBOX_FLY_HEIGHT);
	CEnemy::GetBoundingBox(left, top, right, bottom);
}

void CParagoomba::OnChangeLevel() {
	switch (level)
	{
		case 0: { //die
			die_start = GetTickCount64();
			vx = 0;
			vy = 0;
			ay = 0;
			ax = 0;
			enemyAnimationId = ANI_PARAGOOMBA_DIE;
			break;
		}
		case 1: { //bi dap dau roi
			enemyAnimationId = ANI_PARAGOOMBA_BEINGATTACKED;
			vx = nx * ENEMY_MOVE_SPEED;
			break;
		}
		case 2: { //di
			enemyAnimationId = ANI_PARAGOOMBA_MOVE;
			vx = nx * ENEMY_MOVE_SPEED;
			break;
		}
		case 3: { //nhay thap
			enemyAnimationId = ANI_PARAGOOMBA_FLAP;
			vx = nx * ENEMY_MOVE_SPEED;
			vy = -ENEMY_MOVE_SPEED * 6;
			break;
		}
		case 4: { //nhay cao
			enemyAnimationId = ANI_PARAGOOMBA_FLAP;
			vx = nx * ENEMY_MOVE_SPEED;
			vy = -ENEMY_MOVE_SPEED * 12;
			break;
		}
		default: {
			break;
		}
	}
}

void CParagoomba::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (dynamic_cast<CMario*>(e->obj) && e->ny > 0) {
		if (level >= 2) {
			level = 1;
			OnChangeLevel();
			return;
		}
	}
	CEnemy::OnCollisionWith(e);
}

void CParagoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//di 3 nhip(3s), nhay thap 3 nhip(3s), nhay cao 1 nhip(1s)
	// level 2 -> 3 -> 4 -> 2 -> 3
	//DebugOut(L"time: %, \n", GetTickCount64());
	
	//std::cout<<GetTickCount64()<< std::endl;

	if (level > 1) {
		ULONGLONG time = GetTickCount64() / 1000;
		if (time % T >= 1 && time % T <= (T * 3) / TOTAL_STEPS) {
			if (level == 4) {
				y = y + 5;// 4->2
			}
			level = 2;
			OnChangeLevel();
			//DebugOut(L"di\n");
		}
		else if (isOnPlatform) {
			if (time % T >= (T * 4) / TOTAL_STEPS && time % T <= (T * 6) / TOTAL_STEPS) {
				if (level == 2) {
					y = y - 7;//2->3
				}
				level = 3;
				OnChangeLevel();
				//DebugOut(L"nhay thap\n");
			}
			else {
				level = 4;
				OnChangeLevel();
				//DebugOut(L"nhay cao\n");
			}
		}
		//DebugOut(L"vx: %f, ax: %f, nx: %i\n", vx, ax, nx);
		//DebugOut(L"level: %i \n", level);
	}
	
	CEnemy::Update(dt, coObjects);
}

string CParagoomba::GetAnimationFromState() {
	switch (level)
	{
		case 0: { //die
			return ANI_PARAGOOMBA_DIE;
		}
		case 1: { //bi dap dau roi
			return ANI_PARAGOOMBA_BEINGATTACKED;
		}
		case 2: { //di
			return ANI_PARAGOOMBA_MOVE;
		}
		case 3: { //nhay thap
			return ANI_PARAGOOMBA_FLAP;
		}
		case 4: { //nhay cao
			return ANI_PARAGOOMBA_FLAP;
		}
		default: {
			break;
		}
	}
}