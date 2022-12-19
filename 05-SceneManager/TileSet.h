#pragma once
#include "Utils.h"
#include <memory>
#include <unordered_map>
#include "Textures.h"

class CTileSet
{
	int firstgid;

	D3DXVECTOR2 tileSize; //size 1 o cat

	int tileCount; // so luong tile trong set sau khi cat
	int columns; // so cot trong tile set.
	std::string name;

	LPTEXTURE texture;

	unordered_map<int, std::shared_ptr<RECT>> blocks;

public:
	CTileSet(int firstgid, D3DXVECTOR2 tileSize, int tileCount, int columns, string imgPath);
	CTileSet(TiXmlElement* data, string xmlPath);

	int GetFirstGID();
	D3DXVECTOR2 GetRangeId();

	std::shared_ptr<RECT> GetBlockBoundingBox(int id);

	void Draw(int gid, D3DXVECTOR2 finalPos);

	~CTileSet();
};

typedef CTileSet* LPTileSet;

