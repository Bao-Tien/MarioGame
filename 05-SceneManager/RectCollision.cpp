#include "RectCollision.h"

CRectCollision::CRectCollision(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void CRectCollision::RenderBoundingBox() {
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - 48 / 2 + rect.right / 2;
	float yy = y - 48 / 2 + rect.bottom / 2;

	CGame::GetInstance()->Draw(xx - cx, yy - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CRectCollision::Render()
{

	RenderBoundingBox();
}

void CRectCollision::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = 48 / 2;
	l = x - cellWidth_div_2;
	t = y - cellWidth_div_2;
	r = l + width;
	b = t + height;
}