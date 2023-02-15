#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "SelectionScene.h"
#include "Utils.h"
#include "Sprites.h"
#include "BigMario.h"

#include "SampleKeyEventHandler.h"
#include "MarioSelectionScene.h"

using namespace std;

CSelectionScene::CSelectionScene(string id, string filePath) :
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

void CSelectionScene::Load() {
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
	map = CGameMap().LoadFromTMXFile(loadMapPath, &collisionObjects, &noCollisionObjects);

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

	CMarioSelectionScene* obj = NULL;
	//CMario* obj = NULL;
	if (player != NULL)
	{
		DebugOut(L"[ERROR] MARIO object was created before!\n");
		return;
	}

	if (type_Player == "selectionSceneMario") {
		obj = new CMarioSelectionScene(player_x, player_y);
	}

	/*if (type_Player == "bigMario") {
		obj = new CBigMario(player_x, player_y, player_nx);
	}*/

	player = obj;

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", sceneFilePath);
	CGame::GetInstance()->StartBeginEffect();
}

void CSelectionScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	map->Update(dt);

	for (size_t i = 0; i < collisionObjects.size(); i++)
	{
		collisionObjects[i]->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < collisionObjects.size(); i++)
	{
		coObjects.push_back(collisionObjects[i]);
	}


	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Xet va cham voi Mario
	player->Update(dt, &coObjects);


	// Update camera

	CGame::GetInstance()->SetCamPos(0, 0);

	PurgeDeletedObjects();
	//DebugOut(L"dt %i\n", dt);
}

void CSelectionScene::Render()
{
	// Render Map
	map->Render();

	// Render staticObjects
	for (int i = 0; i < noCollisionObjects.size(); i++)
	{
		if (!noCollisionObjects[i]->GetIsHidden())
			noCollisionObjects[i]->Render();
	}

	for (int i = 0; i < collisionObjects.size(); i++)
	{
		if (!collisionObjects[i]->GetIsHidden())
			collisionObjects[i]->Render();
	}

	// Render Mario
	this->player->Render();
}

/*
*	Clear all objects from this scene
*/
void CSelectionScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = noCollisionObjects.begin(); it != noCollisionObjects.end(); it++)
	{
		delete (*it);
	}
	noCollisionObjects.clear();

	for (it = collisionObjects.begin(); it != collisionObjects.end(); it++)
	{
		delete (*it);
	}
	collisionObjects.clear();

	player = NULL;
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CSelectionScene::Unload()
{
	// Unload staticObjects
	for (int i = 0; i < noCollisionObjects.size(); i++) {
		delete noCollisionObjects[i];
	}
	noCollisionObjects.clear();

	for (int i = 0; i < collisionObjects.size(); i++) {
		delete collisionObjects[i];
	}
	collisionObjects.clear();

	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CSelectionScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CSelectionScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items

	// Static Object
	for (it = noCollisionObjects.begin(); it != noCollisionObjects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	noCollisionObjects.erase(
		std::remove_if(noCollisionObjects.begin(), noCollisionObjects.end(), CSelectionScene::IsGameObjectDeleted),
		noCollisionObjects.end());

	for (it = collisionObjects.begin(); it != collisionObjects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	collisionObjects.erase(
		std::remove_if(collisionObjects.begin(), collisionObjects.end(), CSelectionScene::IsGameObjectDeleted),
		collisionObjects.end());
}