#include "Gate.h"
#include "Mario.h"

void CGate::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
}