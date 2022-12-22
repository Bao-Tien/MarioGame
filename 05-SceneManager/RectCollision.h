#pragma once
#include "GameObject.h"

class CRectCollision : public CGameObject
{
protected:
	

public:
	CRectCollision(int x, int y, int width, int height);
	void Render() {
		CGameObject::Render();
		CGameObject::RenderBoundingBox();
	}
};

