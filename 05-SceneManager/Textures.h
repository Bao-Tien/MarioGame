#pragma once
#include <unordered_map>
#include <d3dx10.h>

#include "Texture.h"
#include "Utils.h"

using namespace std;

/*
	Manage texture database
*/
class CTextures
{
	static CTextures* __instance;
	unordered_map<string, LPTEXTURE> textures2;

public:
	CTextures();
	void Add(string id, LPCWSTR filePath);
	LPTEXTURE Get(string i);
	void LoadTextures(string path, string id);
	void Clear();

	static CTextures* GetInstance();
};