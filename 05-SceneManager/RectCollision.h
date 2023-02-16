#pragma once
#include "GameObject.h"

class CRectCollision : public CGameObject
{
public:
	CRectCollision(int x, int y, int width, int height) {
		this->x = x;
		this->y = y;
		this->BoundingBox_Width = width;
		this->BoundingBox_Height = height;
		isCollidable = 1;
		isBlocking = 1;
	}
	void Render() {
		CGameObject::RenderBoundingBox();
	}
};

