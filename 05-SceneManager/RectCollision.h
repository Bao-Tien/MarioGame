#pragma once
#include "GameObject.h"

class CRectCollision : public CGameObject
{
protected:
	int width, height;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

public:
	CRectCollision(int x, int y, int width, int height);
	void RenderBoundingBox();
	virtual void Render();
};

