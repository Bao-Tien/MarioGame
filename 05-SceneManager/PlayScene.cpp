#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Goomba1.h"
#include "RaccoonMario.h"
#include "FireMario.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}

CPlayScene::CPlayScene(string id, string filePath) :
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::Load() {
	TiXmlDocument document(sceneFilePath2.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return;
	}

	TiXmlElement* root = document.RootElement();

	//load filemap
	TiXmlElement* loadMap = root->FirstChildElement("Map");
	string loadMapPath = loadMap->Attribute("path");
	OutputDebugStringW(ToLPCWSTR("MapPath : " + loadMapPath + '\n'));
	map = CGameMap().LoadFromTMXFile(loadMapPath, this, &staticObjects, &dynamicObjectsFrontMap, &dynamicObjectsAfterMap , &dynamicTroopasFrontMap, &deathPlatformObjects);
	
	//load texture
	TiXmlElement* textures = root->FirstChildElement("Textures");
	for (TiXmlElement* node = textures->FirstChildElement("Texture"); node != nullptr; node = node->NextSiblingElement("Texture"))
	{
		string TexturePath = node->Attribute("path");
		string TextureId = node->Attribute("id");

		CTextures::GetInstance()->LoadTextures(TexturePath, TextureId);

	}

	//load Animation
	TiXmlElement* animations = root->FirstChildElement("Animations");
	for (TiXmlElement* node = animations->FirstChildElement("Animation"); node != nullptr; node = node->NextSiblingElement("Animation"))
	{
		string AnimationPath = node->Attribute("path");
		CAnimations::GetInstance()->LoadAnimations(AnimationPath);
	}

	//load player
	TiXmlElement* play = root->FirstChildElement("Player");
	string type_Player = play->Attribute("type");
	float player_x = stof(play->Attribute("x"));
	float player_y = stof(play->Attribute("y"));
	int player_nx = stoi(play->Attribute("nx"));

	CMario* obj = NULL;
	if (player != NULL)
	{
		DebugOut(L"[ERROR] MARIO object was created before!\n");
		return;
	}
	
	if (type_Player == "bigMario") {
		obj = new CBigMario(player_x, player_y, player_nx);
	}
	else if (type_Player == "raccoonMario") {
		obj = new CRaccoonMario(player_x, player_y, player_nx);
	}
	else {
		obj = new CSmallMario(player_x, player_y, player_nx);
	}
	
	player = obj;

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", sceneFilePath);
	CGame::GetInstance()->StartBeginEffect();
}

void CPlayScene::UpdatePlayer() {
	EMario_Level actualLevel = player->GetLevel();

	EMario_Level currentLevel = EMario_Level::DIE;
	if (dynamic_cast<CSmallMario*>(player)) {
		currentLevel = EMario_Level::SMALL;
	}
	else if (dynamic_cast<CBigMario*>(player)) {
		currentLevel = EMario_Level::BIG;
	}
	else if (dynamic_cast<CRaccoonMario*>(player)) {
		currentLevel = EMario_Level::RACCOON;
	}
	else if (dynamic_cast<CFireMario*>(player)) {
		currentLevel = EMario_Level::FIRE;
	}
	
	if (actualLevel != currentLevel) {
		D3DXVECTOR2 position = player->GetPosition();
		int nx = player->GetNx();
		D3DXVECTOR2 oldBBox = player->GetBoundingBoxSize();
		int coin = player->GetCoin();
		int point = player->GetPoint();
		if (actualLevel == EMario_Level::SMALL) {
			delete(player);
			player = new CSmallMario(position.x, position.y, nx, coin, point);
		}
		else if (actualLevel == EMario_Level::BIG) {
			delete(player);
			player = new CBigMario(position.x, position.y, nx, coin, point);
		}
		else if (actualLevel == EMario_Level::RACCOON) {
			delete(player);
			player = new CRaccoonMario(position.x, position.y, nx, coin, point);
		}
		else if (actualLevel == EMario_Level::FIRE) {
			delete(player);
			player = new CFireMario(position.x, position.y, nx, coin, point);
		}
		player->SetPrevBoundBoxSize(oldBBox);

		if (currentLevel != actualLevel) {
			player->StartUntouchable();
		}
	}
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	
	vector<LPGAMEOBJECT> coObjects;
	map->Update(dt);

	// Xet va cham voi Enemy
	for (size_t i = 0; i < dynamicObjectsAfterMap.size(); i++)
	{
		this->UpdateIfInCameraArea(dynamicObjectsAfterMap[i], dt, &coObjects);
	}
	for (size_t i = 0; i < deathPlatformObjects.size(); i++)
	{
		deathPlatformObjects[i]->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < deathPlatformObjects.size(); i++)
	{
		coObjects.push_back(deathPlatformObjects[i]);
	}
	for (size_t i = 0; i < staticObjects.size(); i++)
	{
		this->UpdateIfInCameraArea(staticObjects[i], dt, &coObjects);
	}
	for (size_t i = 0; i < staticObjects.size(); i++)
	{
		coObjects.push_back(staticObjects[i]);
	}
	for (size_t i = 0; i < dynamicObjectsFrontMap.size(); i++)
	{
		this->UpdateIfInCameraArea(dynamicObjectsFrontMap[i], dt, &coObjects);
	}


	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Xet va cham voi Mario
	for (size_t i = 0; i < dynamicObjectsFrontMap.size(); i++)
	{
		coObjects.push_back(dynamicObjectsFrontMap[i]);
	}
	for (size_t i = 0; i < dynamicTroopasFrontMap.size(); i++)
	{
		this->UpdateIfInCameraArea(dynamicTroopasFrontMap[i], dt, &coObjects);
	}
	for (size_t i = 0; i < dynamicTroopasFrontMap.size(); i++)
	{
		coObjects.push_back(dynamicTroopasFrontMap[i]);
	}
	for (size_t i = 0; i < dynamicObjectsAfterMap.size(); i++)
	{
		coObjects.push_back(dynamicObjectsAfterMap[i]);
	}
	UpdatePlayer();
	player->Update(dt, &coObjects);
	

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;

	if (cx < 0) cx = 0;
	if (cx + game->GetBackBufferWidth() > game->GetCurrentScene2()->GetMapSize().x) {
		cx = game->GetCurrentScene2()->GetMapSize().x - game->GetBackBufferWidth();
	}
	if (cy < 0) cy = 0;

	if (player->GetState() != EMario_State::DIE) {
		if (player->GetPosition().y < 700.0f) {
			CGame::GetInstance()->SetCamPos(cx, cy);
		}
		else if (player->GetPosition().y >= 700.0f && player->GetPosition().y < 1300.0f) {
			CGame::GetInstance()->SetCamPos(cx, 700.0f);
		}
		else {
			CGame::GetInstance()->SetCamPos(cx, 1390.0f);
		}
	}

	PurgeDeletedObjects();
	//DebugOut(L"dt %i\n", dt);

	if (player->GetState() == EMario_State::DIE || player->IsWin() == true) {
		if (mario_die_start == -1) {
			mario_die_start = GetTickCount64();
			CGame::GetInstance()->StartCloseEffect();
		}
		else {
			if (GetTickCount64() - mario_die_start > DIE_TIME) {
				CGame::GetInstance()->InitiateSwitchScene("selectionScene");
				CGame::GetInstance()->SwitchScene2();
			}
		}
	}
}

void CPlayScene::Render()
{
	// Render enemyObjectsBehindMap
	// ...

	for (int i = 0; i < dynamicObjectsAfterMap.size(); i++) {
		this->RenderIfInCameraArea(dynamicObjectsAfterMap[i]);
	}
	// Render Map
	map->Render();
	// Render staticObjects
	for (int i = 0; i < deathPlatformObjects.size(); i++)
	{
		if (!staticObjects[i]->GetIsHidden()) {
			deathPlatformObjects[i]->Render();
		}
	}
	for (int i = 0; i < staticObjects.size(); i++)
	{
		if (!staticObjects[i]->GetIsHidden()) {
			this->RenderIfInCameraArea(staticObjects[i]);
		}
	}

	// Render ObjectsInfrontOfMap
	for (int i = 0; i < dynamicObjectsFrontMap.size(); i++) {
		this->RenderIfInCameraArea(dynamicObjectsFrontMap[i]);
	}
		

	// Render TroopasInFrontMap
	for (int i = 0; i < dynamicTroopasFrontMap.size(); i++) {
		this->RenderIfInCameraArea(dynamicTroopasFrontMap[i]);
	}
		

	// Render Mario
	this->player->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = dynamicObjectsFrontMap.begin(); it != dynamicObjectsFrontMap.end(); it++)
	{
		delete (*it);
	}
	dynamicObjectsFrontMap.clear();

	for (it = dynamicObjectsAfterMap.begin(); it != dynamicObjectsAfterMap.end(); it++)
	{
		delete (*it);
	}
	dynamicObjectsAfterMap.clear();
	for (it = dynamicTroopasFrontMap.begin(); it != dynamicTroopasFrontMap.end(); it++)
	{
		delete (*it);
	}
	dynamicTroopasFrontMap.clear();

	for (it = staticObjects.begin(); it != staticObjects.end(); it++)
	{
		delete (*it);
	}
	staticObjects.clear();

	for (it = deathPlatformObjects.begin(); it != deathPlatformObjects.end(); it++)
	{
		delete (*it);
	}
	deathPlatformObjects.clear();

	player = NULL;
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	CleanObjList(dynamicObjectsFrontMap);
	CleanObjList(dynamicObjectsAfterMap);
	CleanObjList(dynamicTroopasFrontMap);
	CleanObjList(staticObjects);
	CleanObjList(deathPlatformObjects);

	delete player;
	player = NULL;

	map.reset();


	this->Clear();

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	// Enemy Object
	for (it = dynamicObjectsFrontMap.begin(); it != dynamicObjectsFrontMap.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	dynamicObjectsFrontMap.erase(
		std::remove_if(dynamicObjectsFrontMap.begin(), dynamicObjectsFrontMap.end(), CPlayScene::IsGameObjectDeleted),
		dynamicObjectsFrontMap.end());

	// Static Object
	for (it = staticObjects.begin(); it != staticObjects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	staticObjects.erase(
		std::remove_if(staticObjects.begin(), staticObjects.end(), CPlayScene::IsGameObjectDeleted),
		staticObjects.end());

	// dynamicAfterMap Object
	for (it = dynamicObjectsAfterMap.begin(); it != dynamicObjectsAfterMap.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	dynamicObjectsAfterMap.erase(
		std::remove_if(dynamicObjectsAfterMap.begin(), dynamicObjectsAfterMap.end(), CPlayScene::IsGameObjectDeleted),
		dynamicObjectsAfterMap.end());

	// dynamicTroopasFrontMap
	for (it = dynamicTroopasFrontMap.begin(); it != dynamicTroopasFrontMap.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	dynamicTroopasFrontMap.erase(
		std::remove_if(dynamicTroopasFrontMap.begin(), dynamicTroopasFrontMap.end(), CPlayScene::IsGameObjectDeleted),
		dynamicTroopasFrontMap.end());

	// deathPlatformObjects
	for (it = deathPlatformObjects.begin(); it != deathPlatformObjects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	deathPlatformObjects.erase(
		std::remove_if(deathPlatformObjects.begin(), deathPlatformObjects.end(), CPlayScene::IsGameObjectDeleted),
		deathPlatformObjects.end());
}