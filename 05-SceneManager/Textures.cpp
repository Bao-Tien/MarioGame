#include <Windows.h>

#include "debug.h"
#include "Game.h"
#include "textures.h"

CTextures* CTextures::__instance = NULL;

CTextures::CTextures()
{

}

CTextures* CTextures::GetInstance()
{
	if (__instance == NULL) __instance = new CTextures();
	return __instance;
}


void CTextures::Add(string id, LPCWSTR filePath)
{
	textures2[id] = CGame::GetInstance()->LoadTexture(filePath);
}

LPTEXTURE CTextures::Get(string i)
{
	LPTEXTURE t = textures2[i];
	if (t == NULL)
		DebugOut(L"[ERROR] Texture Id %s not found !\n", i);

	return t;
}

void CTextures::LoadTextures(string path, string id)
{
	Add(id, ToLPCWSTR(path));
}

/*
	Clear all loaded textures
*/
void CTextures::Clear()
{
	//textures2
	for (auto x : textures2)
	{
		LPTEXTURE tex = x.second;
		if (tex != NULL) delete tex;
	}
	textures2.clear();
}



