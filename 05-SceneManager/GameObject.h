#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Sprites.h"
#include "Collision.h"

using namespace std;

#define ID_TEX_BBOX "tex-bbox"//-100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency


class CGameObject
{
protected:

	float x; 
	float y;

	float flipX = 1.0f;
	float flipY = 1.0f;

	float vx;
	float vy;

	int nx;	 

	int state;

	bool isDeleted; 
	bool isHidden = false;

	int BoundingBox_Width = 0;
	int BoundingBox_Height = 0;



	D3DXVECTOR2 prevBoundingBoxSize = D3DXVECTOR2(0, 0);
	
	virtual void BeforeSetBoundBox() {
		prevBoundingBoxSize = GetBoundingBoxSize();
	}

	virtual void AfterSetBoundBox() {
		D3DXVECTOR2 currentBoundingBoxSize = GetBoundingBoxSize();
		x = x + (prevBoundingBoxSize.x - currentBoundingBoxSize.x) / 2;
		y = y + (prevBoundingBoxSize.y - currentBoundingBoxSize.y) / 2;
	}


public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }
	virtual void Delete() { isDeleted = true;  }
	bool IsDeleted() { return isDeleted; }
	bool GetIsHidden() { return isHidden; }

	void RenderBoundingBox();

	CGameObject();
	CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; }

	void SetBoundingBoxSize(int width, int height) {
		this->BeforeSetBoundBox();
		this->BoundingBox_Width = width;
		this->BoundingBox_Height = height;
		this->AfterSetBoundBox();
	}
	D3DXVECTOR2 GetBoundingBoxSize() {
		return D3DXVECTOR2(this->BoundingBox_Width, this->BoundingBox_Height);
	}

	

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = this->x - this->BoundingBox_Width / 2;
		top = this->y - this->BoundingBox_Height / 2;
		right = this->x + this->BoundingBox_Width / 2;
		bottom = this->y + this->BoundingBox_Height / 2;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render();
	virtual void SetState(int state) { this->state = state; }
	virtual string GetAnimationFromState() { return ""; };

	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};
	
	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT &o) { return o->isDeleted; }
};

typedef CGameObject* LPGAMEOBJECT;