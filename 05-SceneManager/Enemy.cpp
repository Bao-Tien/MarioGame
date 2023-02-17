#include "Enemy.h"
#include "Mario.h"
#include "RectPlatform.h"
#include "RectCollision.h"
#include "Paragoomba.h"
#include "DeathPlatform.h"
#include "BrickMagic.h"
#include "BrickGolden.h"
#include "TailMario.h"
#include "FireBullet.h"

CEnemy::CEnemy(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->g = ENEMY_GRAVITY;
	die_start = -1;
	nx = -1;
	isOnPlatform = false;
	isAttacked = false;
	isCollidable = 1;
	isBlocking = 0;
}

void CEnemy::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (isAttacked == true) {
		SetBoundingBoxSize(0, 0);
	}

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CEnemy::ActionIsAttacked() {
	// enemy die
	this->flipY = -1;
	this->ay = -ENEMY_ISATTACKED_AY;
	//this->vx = 0;
	this->isAttacked = true;
	this->isAttacked_start = GetTickCount64();
}

void CEnemy::OnCollisionWith(LPCOLLISIONEVENT e)
{

	if (isAttacked == true) {
		return;
	}
	CGameObject::OnCollisionWith(e);

	if (dynamic_cast<CFireBullet*>(e->obj)) {
		if (type == EEnemy_Type::FIRE_FLOWER) {

		}
		else {
			// enemy die
			ActionIsAttacked();
		}
		
	}
	if (!e->obj->IsBlocking())
	{
		return;
	}
	
	//if (dynamic_cast<CEnemy*>(e->obj)) return;

	if (dynamic_cast<CDeathPlatform*>(e->obj)) {
		level = 0;
		OnChangeLevel();
		isDeleted = true;
	}

	if (e->ny < 0) // va cham nen 
	{
		vy = 0;
		isOnPlatform = true;
	}

	if (dynamic_cast<CMario*>(e->obj) || dynamic_cast<CFireBullet*>(e->obj)) {
		int a = 0;
	}
	else {
		if (e->nx != 0)
		{
			nx = nx * -1;
			vx *= nx;
			//DebugOut(L"Quay dau lai: %i\n", nx);
		}
	}

	if (isAutoChangeDirectionWhenMoveOverRangeX == true) {
		if (dynamic_cast<CRectPlatform*>(e->obj) || dynamic_cast<CRectCollision*>(e->obj)) {
			moveRangeX.x = e->obj->GetPosition().x - e->obj->GetBoundingBoxSize().x / 2 + this->BoundingBox_Width / 2;
			moveRangeX.y = e->obj->GetPosition().x + e->obj->GetBoundingBoxSize().x / 2 - this->BoundingBox_Width / 2;
		}
		else if (dynamic_cast<CBrickGolden*>(e->obj) && dynamic_cast<CBrickGolden*>(e->obj)->GetLevel() == 1) {
			if (moveRangeX.x == 0 && moveRangeX.y == 0) {
				moveRangeX.x = e->obj->GetPosition().x - e->obj->GetBoundingBoxSize().x / 2;
				moveRangeX.y = e->obj->GetPosition().x + e->obj->GetBoundingBoxSize().x / 2;
			}
		}
	}

	if (dynamic_cast<CMario*>(e->obj) && level > 0) {
		bool condition1 = e->nx > 0 && this->attackFromLeft == true;
		bool condition2 = e->ny > 0 && this->attackFromTop == true;
		bool condition3 = e->nx < 0 && this->attackFromRight == true;
		bool condition4 = e->ny < 0 && this->attackFromBottom == true;

		if (condition1 || condition2 || condition3 || condition4) {
			// Enemy an toan
		}
		else {
			level--;
			OnChangeLevel();
		}
	}
	
}

void CEnemy::OnOverlapWith(LPCOLLISIONEVENT e) {
	if (dynamic_cast<CTailMario*>(e->obj)) {
		// enemy die
		ActionIsAttacked();
		return;
	}
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if ((level == 0))
	{
		if ((GetTickCount64() - die_start > ENEMY_DIE_TIMEOUT)) {
			isDeleted = true;
		}
		return;
	}
	if (isAttacked) {
		if ((GetTickCount64() - isAttacked_start > 5000)) {
			isDeleted = true;
		}
	}

	vy += (ay + g) * dt;
	vx += ax * dt;

	
	flipX = nx < 0 ? 1 : -1;

	CGameObject::Update(dt, coObjects);
	// reset
	isOnPlatform = false;
	ay = 0;
	CCollision::GetInstance()->Process(this, dt, coObjects);
	
}

void CEnemy::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
}
