#include "FireBullet.h"
#include "Mario.h"
#include "RectCollision.h"
#include "RectPlatform.h"
#include "BrickGolden.h"
#include "Enemy.h"

void CFireBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (level) {
	case 0: {
		SetBoundingBoxSize(0, 0);
		break;
	}
	case 1: {
		SetBoundingBoxSize(FIREBULLET_BBOX_WIDTH, FIREBULLET_BBOX_HEIGHT);
		break;
	}
	}

	CGameObject::GetBoundingBox(left, top, right, bottom);

}

void CFireBullet::OnChangeLevel() {
	switch (level)
	{
		case 0: {
			break;
		}
		case 1: {
			vx = nx * FIREBULLET_VX;
			//ay = ny * FIREBULLET_AY;
			vy = ny * FIREBULLET_VY;
			break;
		}
		default: {
			break;
		}
	}
}

void CFireBullet::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (e->obj) {
		if (dynamic_cast<CRectPlatform*>(e->obj) || dynamic_cast<CRectCollision*>(e->obj) || dynamic_cast<CBrickGolden*>(e->obj)) {
			level = 1;
			ny = -1;
			OnChangeLevel();
		}
		if (dynamic_cast<CEnemy*>(e->obj)) {
			level = 0;
		}
	}
}

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if ((GetTickCount64() - die_start > FIRE_BULLET_DIE_TIMEOUT)) {
		isDeleted = true;
		return;
	}

	//vy += (ay + g) * dt;
	vy += g * dt;
	vx = vx;

	/*if (vy < -MAX_VY) {
		vy = 0;
		ny = 1;
	}*/
	DebugOut(L"vy: %f, ay: %f \n", vy, ay);
	//reset 
	ay = 0;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CFireBullet::Render() {
	CGameObject::Render();
	CGameObject::RenderBoundingBox();
}

string CFireBullet::GetAnimationFromState() {
	switch (level)
	{
		case 0: {
			return "";
		}
		case 1: {
			return ID_ANI_FIREBULLET;
		}
		default: {
			break;
		}
	}
}