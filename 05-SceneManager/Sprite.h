#pragma once

#include "Texture.h"
#include "Game.h"

class CSprite
{
	int id;				// Sprite ID in the sprite database
	string id2;

	int left;
	int top;
	int right;
	int bottom;

	LPTEXTURE texture;
	D3DX10_SPRITE sprite;
public:
	CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex);
	CSprite(string id, int left, int top, int right, int bottom, LPTEXTURE tex);

	void Draw(float x, float y, float flipX = 1.0f, float flipY = 1.0f);
};

typedef CSprite* LPSPRITE;