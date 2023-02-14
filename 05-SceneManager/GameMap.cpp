#include "GameMap.h"
#include "Game.h"
#include "RectCollision.h"
#include "RectPlatform.h"
#include "Goomba1.h"
#include "Troopas.h"
#include "Paragoomba.h"
#include "RedVenus.h"
#include "Coin.h"
#include "BrickMagic.h"
#include "DeathPlatform.h"
#include "TroopasGreen.h"
#include "ParaTroopa.h"
#include "GreenVenus.h"
#include "GreenPiranha.h"
#include "BrickGolden.h"
#include "Gate.h"
#include "EndSceneItem.h"
#include "Tree.h"
#include "GateSelectionScene.h" 
#include "Slave.h"

#define MAX_ENERGY 40
#define CAMERA_MARGIN			150
#define HUD_X 15+250
#define HUD_Y 850
#define EMPTY_CARD_START 600
#define EMPTY_CARD_D 74
#define POINT_START 218
#define ARROW_START 220
#define ARROW_Y 840
#define ARROW_D 23
#define P_START 230
#define TIME_START 423
#define TIME_D 24
#define TIME_Y 865
#define COIN_START 469
#define COIN_D 24
#define COIN_Y 841

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
	sceneTime -= dt;
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
					tilesetPtr->Draw(id, D3DXVECTOR2(x + floor(tileWidth / 2 - camPos.x), floor(y + tileHeight / 2 - camPos.y)));
				}
			}
		}
	}
	RenderHUD();
}



shared_ptr<CGameMap> CGameMap::LoadFromTMXFile(string filePath, CPlayScene* playScene, vector<LPGAMEOBJECT>* staticObjects,
	vector<LPGAMEOBJECT>* dynamicObjectsFrontMap, vector<LPGAMEOBJECT>* dynamicObjectsAfterMap, vector<LPGAMEOBJECT>* dynamicTroopasFrontMap)
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
			else if (std::string(objGroupNode->Attribute("name")) == "DeathPlatform") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CDeathPlatform(
						x + width / 2,
						y + height / 2,
						width,
						height
					);
					staticObjects->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Gate") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj;
					int gateId;
					string gateType;
					TiXmlElement* objProperties = objNode->FirstChildElement("properties");
					if (objProperties != NULL) {
						for (TiXmlElement* objPropertiesNode = objProperties->FirstChildElement("property"); objPropertiesNode != nullptr; objPropertiesNode = objPropertiesNode->NextSiblingElement("property")) {
							string nameProperty = objPropertiesNode->Attribute("name");
							if (nameProperty == "GateId") {
								int valueProperty = atoi(objPropertiesNode->Attribute("value"));
								gateId = valueProperty;
							}
							
							if (nameProperty == "GateType") {
								string valueProperty = objPropertiesNode->Attribute("value");
								gateType = valueProperty;
							}
						}
					}
					obj = new CGate(
						x + width / 2,
						y + height / 2,
						width,
						height,
						gateId,
						gateType
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
					dynamicTroopasFrontMap->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Enemy_TroopasGreen") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CTroopasGreen(
						x + width / 2,
						y + height / 2
					);
					dynamicTroopasFrontMap->push_back(obj);
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
			else if (std::string(objGroupNode->Attribute("name")) == "Enemy_ParaTroopa") {
			for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
				int x = atoi(objNode->Attribute("x"));
				int y = atoi(objNode->Attribute("y"));
				int width = atoi(objNode->Attribute("width"));
				int height = atoi(objNode->Attribute("height"));

				LPGAMEOBJECT obj = new CParaTroopa(
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
					staticObjects->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "EndSceneItem") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CEndSceneItem(
						x + width / 2,
						y + height / 2
					);
					staticObjects->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Brick_Magic") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj;
					string gift;
					string typeBrick;
					TiXmlElement* objProperties = objNode->FirstChildElement("properties");
					if (objProperties != NULL) {
						for (TiXmlElement* objPropertiesNode = objProperties->FirstChildElement("property"); objPropertiesNode != nullptr; objPropertiesNode = objPropertiesNode->NextSiblingElement("property")) {
							string typeProperty = objPropertiesNode->Attribute("name");
							string valueProperty = objPropertiesNode->Attribute("value");
							if (typeProperty == "Gift") gift = valueProperty;
							if (typeProperty == "Type") typeBrick = valueProperty;
						}
					}
					obj = new CBrickMagic(
						x + width / 2,
						y + height / 2,
						playScene,
						typeBrick,
						gift
						);
					
					staticObjects->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Brick_Golden") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CBrickGolden(
						x + width / 2,
						y + height / 2
					);
					staticObjects->push_back(obj);
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
						y + height / 2,
						playScene
					);
					dynamicObjectsAfterMap->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Enemy_GreenVenus") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CGreenVenus(
						x + width / 2,
						y + height / 2,
						playScene
					);
					dynamicObjectsAfterMap->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Enemy_GreenPiranha") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CGreenPiranha(
						x + width / 2,
						y + height / 2,
						playScene
					);
					dynamicObjectsAfterMap->push_back(obj);
				}
			}
		}

		return gameMap;
	}
	throw "Load map fail!!!!!!!!";
}

shared_ptr<CGameMap> CGameMap::LoadFromTMXFile(string filePath, vector<LPGAMEOBJECT>* staticObjects)
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
			else if (std::string(objGroupNode->Attribute("name")) == "Tree") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CTree(
						x + width / 2,
						y + height / 2
					);
					staticObjects->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Gate") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj;
					int gateId;
					TiXmlElement* objProperties = objNode->FirstChildElement("properties");
					if (objProperties != NULL) {
						for (TiXmlElement* objPropertiesNode = objProperties->FirstChildElement("property"); objPropertiesNode != nullptr; objPropertiesNode = objPropertiesNode->NextSiblingElement("property")) {
							string nameProperty = objPropertiesNode->Attribute("name");
							if (nameProperty == "Id") {
								int valueProperty = atoi(objPropertiesNode->Attribute("value"));
								gateId = valueProperty;
							}
						}
					}
					obj = new CGateSelectionScene(
						x + width / 2,
						y + height / 2,
						gateId
					);

					staticObjects->push_back(obj);
				}
			}
			else if (std::string(objGroupNode->Attribute("name")) == "Slave") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					int x = atoi(objNode->Attribute("x"));
					int y = atoi(objNode->Attribute("y"));
					int width = atoi(objNode->Attribute("width"));
					int height = atoi(objNode->Attribute("height"));

					LPGAMEOBJECT obj = new CSlave(
						x + width / 2,
						y + height / 2
					);
					staticObjects->push_back(obj);
				}
			}
		}
		return gameMap;
	}
	throw "Load map fail!!!!!!!!";
}

void CGameMap::RenderHUD() {
	//hud
	CAnimations::GetInstance()->Get("ani-hud")->RenderFixed(HUD_X, HUD_Y);
	ULONGLONG currentTime = GetTickCount64();
	// nang luong - 6 tam giac + 1P
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene2())->GetPlayer();
	CGameObject* player = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene2())->GetPlayer();
	float energy;
	int intCoin;
	int intPoint;
	if (dynamic_cast<CMario*>(player)) {
		energy = mario->GetEnergy();
		intCoin = mario->GetCoin();
		intPoint = mario->GetPoint();
	}
	else {
		energy = 0;
		intCoin = 0;
		intPoint = 0;
	}
	 
	if (energy == 0) {
		for (int i = 0; i < 7; i++) {
			if (i == 6) {
				CAnimations::GetInstance()->Get("ani-p-icon-black")->RenderFixed(P_START + i * ARROW_D, ARROW_Y);
			}
			else {
				CAnimations::GetInstance()->Get("ani-arrow-icon-black")->RenderFixed(ARROW_START + i * ARROW_D, ARROW_Y);

			}
		}
	}
	else {
		int energy_index = energy / MAX_ENERGY * 7;
		for (int i = 0; i < energy_index; i++) {
			if (i == 6) {
				if (currentTime % 2 == 0) {
					CAnimations::GetInstance()->Get("ani-p-icon-white")->RenderFixed(P_START + i * ARROW_D, ARROW_Y);
				}
				else {
					CAnimations::GetInstance()->Get("ani-p-icon-black")->RenderFixed(P_START + i * ARROW_D, ARROW_Y);
				}
				
			}
			else {
				CAnimations::GetInstance()->Get("ani-arrow-icon-white")->RenderFixed(ARROW_START + i * ARROW_D, ARROW_Y);
			}
		}
		if (energy_index < 7) {
			for (int i = energy_index; i < 7; i++) {
				if (i == 6) {
					CAnimations::GetInstance()->Get("ani-p-icon-black")->RenderFixed(P_START + i * ARROW_D, ARROW_Y);
				}
				else {
					CAnimations::GetInstance()->Get("ani-arrow-icon-black")->RenderFixed(ARROW_START + i * ARROW_D, ARROW_Y);
				}
			}
		}
	}
	
	// so dong tien
	
	string coin = std::to_string(intCoin);
	if (intCoin != 0) {
		for (int i = 0; i < coin.length(); i++) {
			string a = "spr-font-";
			char b = coin[i];
			string spriteId = a + b;
			if (intCoin < 10) {
				CSprites::GetInstance()->Get(spriteId)->DrawFixed(COIN_START + COIN_D * i, COIN_Y);
			}
			else {
				CSprites::GetInstance()->Get(spriteId)->DrawFixed(COIN_START - COIN_D + COIN_D * i, COIN_Y);
			}
		}
	}
	// diem
	
	string point = std::to_string(intPoint);
	if (intPoint == 0) {
		point = "0000000";
		for (int i = 0; i < point.length(); i++) {
			string spriteId = "spr-font-0";
			CSprites::GetInstance()->Get(spriteId)->DrawFixed(POINT_START + ARROW_D * i, TIME_Y);
		}
	}
	else {
		for (int i = 0; i < 7 - point.length(); i++) {
			string spriteId = "spr-font-0";
			CSprites::GetInstance()->Get(spriteId)->DrawFixed(POINT_START + ARROW_D * i, TIME_Y);
		}
		int x = 0;
		int index = 7 - point.length();
		for (int i = index; i < 7; i++) {
			string a = "spr-font-";
			char b = point[x];
			string spriteId = a + b;
			CSprites::GetInstance()->Get(spriteId)->DrawFixed(POINT_START + ARROW_D * i, TIME_Y);
			x++;
		}
	}
	// thoi gian
	string time = std::to_string(sceneTime/1000);
	for (int i = 0; i < time.length(); i++) {
		string a = "spr-font-";
		char b = time[i];
		string spriteId = a + b;
		CSprites::GetInstance()->Get(spriteId)->DrawFixed(TIME_START + TIME_D * i, TIME_Y);
	}
	
	// 3 card empty
	for (int i = 0; i < 3; i++) {
		CAnimations::GetInstance()->Get("ani-empty-card")->RenderFixed(EMPTY_CARD_START + i * EMPTY_CARD_D, HUD_Y);
	}
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
