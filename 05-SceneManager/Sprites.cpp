#include "Sprites.h"
#include "Game.h"
#include "debug.h"
#include "Textures.h"

CSprites* CSprites::__instance = NULL;

CSprites* CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprites::Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}

void CSprites::Add2(string id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites2[id] = s;
}

LPSPRITE CSprites::Get(string id)
{
	return sprites2[id];
}

void CSprites::LoadSprites(TiXmlElement* sprNode, string textureID, string& spriteID, int& detailFrameTime)
{
	LPTEXTURE tex = CTextures::GetInstance()->Get(textureID);
	spriteID = sprNode->Attribute("id");
	detailFrameTime = 100;
	int left = 0;
	int top = 0;
	int width = 0;
	int height = 0;
	sprNode->QueryIntAttribute("frameTime", &detailFrameTime);
	sprNode->QueryIntAttribute("left", &left);
	sprNode->QueryIntAttribute("top", &top);
	sprNode->QueryIntAttribute("width", &width);
	sprNode->QueryIntAttribute("height", &height);
	OutputDebugStringW(ToLPCWSTR(spriteID + ':' + to_string(left) + ':' + to_string(top) + ':' + to_string(width) + ':' + to_string(height) + '\n'));
	Add2(spriteID, left * 3, top * 3, (width + left) * 3, (height + top) * 3, tex);

	//OutputDebugStringW(ToLPCWSTR("|--" + spriteID + ':' + to_string(detailFrameTime) + '\n'));
}
/*
	Clear all loaded sprites
*/
void CSprites::Clear()
{
	//sprites
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}
	sprites.clear();

	//sprites2
	for (auto x : sprites2)
	{
		LPSPRITE s = x.second;
		delete s;
	}
	sprites2.clear();
}