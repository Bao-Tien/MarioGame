#include "RectCollision.h"

CRectCollision::CRectCollision(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void CRectCollision::Render()
{
	RenderBoundingBox();
}

void CRectCollision::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}