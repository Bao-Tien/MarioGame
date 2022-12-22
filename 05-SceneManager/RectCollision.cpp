#include "RectCollision.h"

CRectCollision::CRectCollision(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->BoundingBox_Width = width;
	this->BoundingBox_Height = height;
}

