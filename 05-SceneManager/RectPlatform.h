#pragma once
#include "GameObject.h"

class CRectPlatform : public CGameObject
{
public:
	CRectPlatform(int x, int y, int width, int height) {
		this->x = x;
		this->y = y;
		this->BoundingBox_Width = width;
		this->BoundingBox_Height = height;
	}

	void Render() override {
		CGameObject::RenderBoundingBox();
	}

	virtual int IsBlockingLeft() override { return 0; }
	virtual int IsBlockingRight() override { return 0; }
	virtual int IsBlockingBottom() override { return 0; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};