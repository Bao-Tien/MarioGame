#pragma once
#include "GameObject.h"

#define ID_ANI_FIREBULLET "ani-fire-bullet"
#define FIREBULLET_BBOX_WIDTH 30
#define FIREBULLET_BBOX_HEIGHT 30

#define FIREBULLET_VY 0.7f
#define FIREBULLET_VX 0.3f
#define MAX_VY 5.0f
#define FIREBULLET_GRAVITY 0.002f
#define FIRE_BULLET_DIE_TIMEOUT 50000

class CFireBullet : public CGameObject
{
protected:
	CPlayScene* playScene;
	ULONGLONG die_start;
	float g;
	int ny;
	float ax;
	float ay;
	int level;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CFireBullet(float x, float y, int nx) : CGameObject(x, y) {
		this->nx = nx;
		ny = -1;
		level = 1;
		OnChangeLevel();
		die_start = GetTickCount64();
		g = FIREBULLET_GRAVITY;
	}
	int IsCollidable() { return 1; }
	int IsBlocking() { return 0; }
	void OnChangeLevel();
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	string GetAnimationFromState();
};