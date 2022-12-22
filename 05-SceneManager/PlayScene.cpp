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
	map = CGameMap().LoadFromTMXFile(loadMapPath, &staticObjects);

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
	CMario* obj = NULL;
	if (player != NULL)
	{
		DebugOut(L"[ERROR] MARIO object was created before!\n");
		return;
	}
	obj = new CBigMario(150.0f, 50.0f);
	player = obj;


	//Goomba
	/*CGoomba* goomba = new CGoomba(280.0f, 100.0f);
	this->enemyObjects.push_back(goomba);*/

	/*CPlatform* p = new CPlatform(100.0f, 1048.0f, 48, 48, 60);
	this->staticObjects.push_back(p);*/

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", sceneFilePath);

}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;

	// Xet va cham voi Enemy
	for (size_t i = 0; i < staticObjects.size(); i++)
	{
		coObjects.push_back(staticObjects[i]);
	}
	for (size_t i = 0; i < enemyObjects.size(); i++)
	{
		enemyObjects[i]->Update(dt, &coObjects);
	}


	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Xet va cham voi Mario
	for (size_t i = 0; i < enemyObjects.size(); i++)
	{
		coObjects.push_back(enemyObjects[i]);
	}
	player->Update(dt, &coObjects);


	

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;

	if (cx < 0) cx = 0;

	CGame::GetInstance()->SetCamPos(cx, 500.0f);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	// Render enemyObjectsBehindMap
	// ...


	// Render Map
	map->Render();
	for (int i = 0; i < staticObjects.size(); i++)
	{
		if (!staticObjects[i]->GetIsHidden())
			staticObjects[i]->Render();
	}

	// Render enemyObjectsInfrontOfMap
	for (int i = 0; i < enemyObjects.size(); i++)
		enemyObjects[i]->Render();

	// Render Mario
	this->player->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = enemyObjects.begin(); it != enemyObjects.end(); it++)
	{
		delete (*it);
	}
	enemyObjects.clear();

	for (it = staticObjects.begin(); it != staticObjects.end(); it++)
	{
		delete (*it);
	}
	staticObjects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	// Unload enemy
	for (int i = 0; i < enemyObjects.size(); i++) {
		delete enemyObjects[i];
	}
	enemyObjects.clear();

	// Unload staticObjects
	for (int i = 0; i < staticObjects.size(); i++) {
		delete staticObjects[i];
	}
	staticObjects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = enemyObjects.begin(); it != enemyObjects.end(); it++)
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
	enemyObjects.erase(
		std::remove_if(enemyObjects.begin(), enemyObjects.end(), CPlayScene::IsGameObjectDeleted),
		enemyObjects.end());
}