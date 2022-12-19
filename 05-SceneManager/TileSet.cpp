#include "TileSet.h"
#include "Game.h"

CTileSet::CTileSet(int firstgid, D3DXVECTOR2 tileSize, int tileCount, int columns, string imgPath)
{
	this->firstgid = firstgid;
	this->tileSize = D3DXVECTOR2(tileSize.x, tileSize.y);
	this->tileCount = tileCount;
	this->columns = columns;
}

CTileSet::CTileSet(TiXmlElement* data, string xmlPath)
{
	data->QueryIntAttribute("firstgid", &this->firstgid);
	data->QueryFloatAttribute("tilewidth", &this->tileSize.x);
	data->QueryFloatAttribute("tileheight", &this->tileSize.y);
	data->QueryIntAttribute("tilecount", &this->tileCount);
	data->QueryIntAttribute("columns", &this->columns);

	this->name = data->Attribute("name");

	TiXmlElement* imgDom = data->FirstChildElement("image");
	string imgPath = imgDom->Attribute("source");
	string nameTileSet = "tileset_" + this->name;
	LPCWSTR pathTileSet = ToLPCWSTR("map-data/Map/World1-1/" + imgPath);
	CTextures::GetInstance()->Add(nameTileSet, pathTileSet);
	this->texture = CTextures::GetInstance()->Get("tileset_" + this->name);
}

int CTileSet::GetFirstGID()
{
	return firstgid;
}

D3DXVECTOR2 CTileSet::GetRangeId()
{
	return D3DXVECTOR2(firstgid, firstgid + tileCount);
}

shared_ptr<RECT> CTileSet::GetBlockBoundingBox(int id)
{
	if (id < firstgid) return nullptr;
	if (blocks.find(id) == blocks.end()) return nullptr;
	return blocks[id];
}

void CTileSet::Draw(int gid, D3DXVECTOR2 finalPos)
{
	// git: stt cua title trong titleset
	if (gid < firstgid) return;
	RECT r;
	r.top = ((gid - firstgid) / columns) * tileSize.y;
	r.left = ((gid - firstgid) % columns) * tileSize.x;
	r.bottom = r.top + tileSize.y;
	r.right = r.left + tileSize.x;

	CGame::GetInstance()->Draw(finalPos.x, finalPos.y, texture, r.left, r.top, r.right, r.bottom);
}

CTileSet::~CTileSet()
{
	if (texture) {
		delete texture;
	}
	texture = NULL;
}
