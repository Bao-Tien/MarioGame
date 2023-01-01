#include "GameMap.h"
#include "Game.h"
#include "RectCollision.h"
#include "RectPlatform.h"
#include "Goomba1.h"
#include "Troopas.h"
#include "Paragoomba.h"
#include "RedVenus.h"
#include "Coin.h"

#define CAMERA_MARGIN			150

CGameMap::CGameMap()
{

}

CGameMap::CGameMap(int width, int height, int tileWidth, int tileHeight)
{
	this->width = width;
	this->height = height;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;
}

D3DXVECTOR2 CGameMap::GetBound()
{
	return D3DXVECTOR2(this->width * tileWidth, this->height * tileHeight);
}

CTileSet* CGameMap::GetTileSetByTileID(int id)
{
	if (!(id >= tRender.rangeId.x && id <= tRender.rangeId.y)) {
		for (int i = 0; i < tilesets.size(); i++) {
			D3DXVECTOR2 tilesetRangeId = tilesets[i]->GetRangeId();
			if (id >= tilesetRangeId.x && id <= tilesetRangeId.y) {
				tRender.rangeId.x = tilesetRangeId.x;
				tRender.rangeId.y = tilesetRangeId.y;
				tRender.tileset = tilesets[i];
			}
		}
	}


	return tRender.tileset;
}

void CGameMap::AddTileSet(int firstgid, CTileSet* tileSet)
{
	this->tilesets[firstgid] = tileSet;
}

void CGameMap::AddLayer(shared_ptr<CMapLayer> layer)
{
	this->layers.push_back(layer);
}

void CGameMap::Update(int dt)
{
	//camPosition = CGame::GetInstance()->camera->GetCamPosition();
}

void CGameMap::Render()
{
	/*int col = CGame::GetInstance()->GetPosCamera().x / tileWidth;
	int row = CGame::GetInstance()->GetPosCamera().y / tileHeight;*/
	int col = 0;
	int row = 0;
	
	if (col > 0) col--;
	if (row > 0) row--;
	if (col < 0) col = 0;
	if (row < 0) row = 0;

	/*D3DXVECTOR2 camSize = D3DXVECTOR2((CGame::GetInstance()->GetBackBufferWidth() + CAMERA_MARGIN) / tileWidth,
								(CGame::GetInstance()->GetBackBufferHeight() + CAMERA_MARGIN) / tileHeight);*/

	//D3DXVECTOR2 deltaSize = D3DXVECTOR2(0, 0);

	//if (camSize.x > width) {
	//	//deltaSize.x = ( camSize.x - width ) / 2 - 1;
	//	camSize.x = width;
	//} 
	//if (camSize.y > height) {
	//	//deltaSize.y = (camSize.y - height) / 2 ;
	//	camSize.y = height;
	//} 

	
	D3DXVECTOR2 camPos = CGame::GetInstance()->GetCamPos();

	for (int i = col; i < width; i++) {
		for (int j = row; j < height; j++) {

			int x = i * tileWidth;
			int y = j * tileHeight;

			for (shared_ptr<CMapLayer> layer : layers) {
				if (layer->Hidden) continue;
				int id = layer->GetTileID(i % width, j % height);
				CTileSet* tilesetPtr = this->GetTileSetByTileID(id);
				if (tilesetPtr != nullptr) {
					tilesetPtr->Draw(id, D3DXVECTOR2(x + tileWidth / 2 - camPos.x, y + tileWidth / 2 - camPos.y));
				}
				
			}
		}
	}
}



shared_ptr<CGameMap> CGameMap::LoadFromTMXFile(string filePath, vector<LPGAMEOBJECT>* staticObjects, 
	vector<LPGAMEOBJECT>* dynamicObjectsFrontMap, vector<LPGAMEOBJECT>* dynamicObjectsAfterMap)
{
	string fullPath = filePath;
	TiXmlDocument doc(fullPath.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();
		shared_ptr<CGameMap> gameMap = shared_ptr<CGameMap>(new CGameMap());

		root->QueryIntAttribute("width", &gameMap->width);
		root->QueryIntAttribute("height", &gameMap->height);
		root->QueryIntAttribute("tilewidth", &gameMap->tileWidth);
		root->QueryIntAttribute("tileheight", &gameMap->tileHeight);

		//Load tileset
		for (TiXmlElement* node = root->FirstChildElement("tileset"); node != nullptr; node = node->NextSiblingElement("tileset")) {
			CTileSet* tileSet = new CTileSet(node, filePath);
			gameMap->tilesets.push_back(tileSet);
		}

		//Load layer
		for (TiXmlElement* node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer")) {
			shared_ptr<CMapLayer> layer = shared_ptr<CMapLayer>(new CMapLayer(node));
			gameMap->AddLayer(layer);
		}

		// Load collision group objects
		for (TiXmlElement* objGroupNode = root->FirstChildElement("objectgroup"); objGroupNode != nullptr; objGroupNode = objGroupNode->NextSiblingElement("objectgroup")) {
			if (std::string(objGroupNode->Attribute("name")) == "RectCollision") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CRectCollision(
						x + width / 2,
						y + height / 2,
						width,
						height
					);
					staticObjects->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "RectPlatform") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CRectPlatform(
						x + width / 2,
						y + height / 2,
						width,
						height
					);
					staticObjects->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Enemy_Goomba") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CGoomba1(
						x + width / 2,
						y + height / 2
					);
					dynamicObjectsFrontMap->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Enemy_Troopas") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CTroopas(
						x + width / 2,
						y + height / 2
					);
					dynamicObjectsFrontMap->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Enemy_Paragoomba") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CParagoomba(
						x + width / 2,
						y + height / 2
					);
					dynamicObjectsFrontMap->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Coin") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CCoin(
						x + width / 2,
						y + height / 2
					);
					dynamicObjectsFrontMap->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Enemy_RedVenus") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CRedVenus(
						x + width / 2,
						y + height / 2
					);
					dynamicObjectsAfterMap->push_back(obj);
				}
			}
		}

		return gameMap;
	}
	throw "Load map fail!!!!!!!!";
}

void CGameMap::GetMapSize(D3DXVECTOR2& out) {
	out.x = this->width * tileWidth;
	out.y = this->height * tileHeight;
}

CGameMap::~CGameMap()
{
	layers.clear();
	tilesets.clear();
}
