#include "Enemy.h"
#include "Mario.h"
#include "RectPlatform.h"
#include "RectCollision.h"
#include "Paragoomba.h"

CEnemy::CEnemy(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = 0;
	this->g = ENEMY_GRAVITY;
	die_start = -1;
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

	if (e->nx != 0 && this->isAutoChangeDirectionWhenHitCollision == true)
	{
		vx = -vx;
	}

	if (isAutoChangeDirectionWhenMoveOverRangeX == true) {
		if (dynamic_cast<CRectPlatform*>(e->obj) || dynamic_cast<CRectCollision*>(e->obj)) {
			if (moveRangeX.x == 0 && moveRangeX.y == 0) {
				moveRangeX.x = e->obj->GetPosition().x - e->obj->GetBoundingBoxSize().x / 2 + this->BoundingBox_Width / 2;
				moveRangeX.y = e->obj->GetPosition().x + e->obj->GetBoundingBoxSize().x / 2 - this->BoundingBox_Width / 2;
			}
		}
	}

	if (dynamic_cast<CMario*>(e->obj) && level > 0) {
		bool condition1 = e->nx < 0 && this->attackFromLeft == true;
		bool condition2 = e->ny < 0 && this->attackFromTop == true;
		bool condition3 = e->nx > 0 && this->attackFromRight == true;
		bool condition4 = e->ny > 0 && this->attackFromBottom == true;

		if (condition1 && condition2 && condition3 && condition4) {
			// Enemy an toan
		}
		else {
			level--;
			OnChangeLevel();
		}
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

	if (dynamic_cast<CParagoomba*>(this)) {
		int a = 9;
	}
	vy += (ay + g) * dt;
	vx += ax * dt;

	if (isAutoChangeDirectionWhenMoveOverRangeX == true) {
		if (moveRangeX.x != 0 && moveRangeX.y != 0) {
			if (x < moveRangeX.x) {
				x = moveRangeX.x;
				nx *= -1;
				vx *= -1;
			}
			if (x > moveRangeX.y) {
				x = moveRangeX.y;
				nx *= -1;
				vx *= -1;
			}
		}
	}
	flipX = nx < 0 ? 1 : -1;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CEnemy::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
}

string CEnemy::GetAnimationFromState() {
	return enemyAnimationId;
}
