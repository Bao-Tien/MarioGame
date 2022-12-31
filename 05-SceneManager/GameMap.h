#pragma once
#include "Utils.h"
#include "TileSet.h"
#include "MapLayer.h"
#include <map>

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct TilesetRender {
	CTileSet* tileset;
	D3DXVECTOR2 rangeId = D3DXVECTOR2(0, 0);
};

class CGameMap
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	D3DXVECTOR2 camPosition;

	TilesetRender tRender;

	vector<CTileSet*> tilesets;
	vector<shared_ptr<CMapLayer>> layers;

public:
	CGameMap();
	CGameMap(int width, int height, int tileWidth, int tileHeight);


	virtual D3DXVECTOR2 GetBound();

	virtual CTileSet* GetTileSetByTileID(int id);

	virtual void AddTileSet(int firstgid, CTileSet* tileSet);

	virtual void AddLayer(shared_ptr<CMapLayer> layer);

	virtual void Update(int dt);

	virtual void Render();

	virtual void GetMapSize(D3DXVECTOR2& out);

	static shared_ptr<CGameMap> LoadFromTMXFile(string filePath, vector<LPGAMEOBJECT>* staticObjects, 
		vector<LPGAMEOBJECT>* dynamicObjectsFrontMap, vector<LPGAMEOBJECT>* dynamicObjectsAfterMap);
	~CGameMap();
};

typedef CGameMap* LPGAMEMAP;
