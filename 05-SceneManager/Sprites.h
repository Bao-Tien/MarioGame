#pragma once
#include <Windows.h>
#include <unordered_map>

#include <d3dx10.h>

#include "Texture.h"
#include "Sprite.h"
#include "Utils.h"

using namespace std;

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites* __instance;

	unordered_map<int, LPSPRITE> sprites;
	unordered_map<string, LPSPRITE> sprites2;

public:
	void Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex);
	LPSPRITE Get(int id);
	void Add2(string id, int left, int top, int right, int bottom, LPTEXTURE tex);
	LPSPRITE Get(string id);
	void LoadSprites(TiXmlElement* sprNode, string textureID, string& spriteID, int& detailFrameTime);
	void Clear();

	static CSprites* GetInstance();
};

