#pragma once
#include "RectPlatform.h"

class CDeathPlatform : public CRectPlatform
{
public:
	CDeathPlatform(int x, int y, int width, int height) : CRectPlatform(x, y, width, height) {
		this->x = x;
		this->y = y;
		this->BoundingBox_Width = width;
		this->BoundingBox_Height = height;
	}
	void Render() {
		CGameObject::RenderBoundingBox();
	}
};