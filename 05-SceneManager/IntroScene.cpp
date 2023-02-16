#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "IntroScene.h"
#include "Utils.h"
#include "Sprites.h"

#include "SampleKeyEventHandler.h"
#include "MarioSelectionScene.h"

using namespace std;

CIntroScene::CIntroScene(string id, string filePath) :
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}

void CIntroScene::Load() {
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

	player = obj;

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", sceneFilePath);
	//CGame::GetInstance()->StartBeginEffect();

	curtain_start = GetTickCount64();
}

void CIntroScene::Update(DWORD dt)
{
	if (change_scene_start != -1 && GetTickCount64() - change_scene_start > CHANGE_SCENE_TIME) {
		if (CGame::GetInstance()->GetCurrentScene2()->GetId() == "introScene") {
			CGame::GetInstance()->InitiateSwitchScene("selectionScene");
			CGame::GetInstance()->SwitchScene2();
		}
		return;
	}

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

void CIntroScene::KeyboardHandle(int KeyCode, EKeyType type) {
	switch (KeyCode)
	{
		case DIK_S:
			if (change_scene_start == -1) {
				change_scene_start = GetTickCount64();
				CGame::GetInstance()->StartCloseEffect();
			}
			break;
		case DIK_Q:
			changeOption *= -1;
			break;
		default:
			break;
	}
}

void CIntroScene::Render()
{
	// Render Map
	map->Render();
	
	CAnimations::GetInstance()->Get("ani-title")->RenderFixed(376, 210);
	CAnimations::GetInstance()->Get("ani-3")->RenderFixed(376, 400);

	CAnimations::GetInstance()->Get("ani-option-0")->RenderFixed(370, 495);
	CAnimations::GetInstance()->Get("ani-option-1")->RenderFixed(370, 545);
	CAnimations::GetInstance()->Get("ani-option-2")->RenderFixed(370, 595);

	if (changeOption == -1) {
		CAnimations::GetInstance()->Get("ani-option-arrow")->RenderFixed(185, 495);
	}
	else {
		CAnimations::GetInstance()->Get("ani-option-arrow")->RenderFixed(185, 545);
	}
	
	float ratio = ((GetTickCount64() - curtain_start) * 1.0f) / (CURTAIN_SCROLL * 1.0f);
	ratio = ratio > 1 ? 1 : ratio;

	CAnimations::GetInstance()->Get("ani-full-curtain")->RenderFixed(360, CURTAIN_Y_START - (CURTAIN_Y_START - CURTAIN_Y_END) * ratio);

	// Render staticObjects
	//for (int i = 0; i < noCollisionObjects.size(); i++)
	//{
	//	if (!noCollisionObjects[i]->GetIsHidden())
	//		noCollisionObjects[i]->Render();
	//}

	//for (int i = 0; i < collisionObjects.size(); i++)
	//{
	//	if (!collisionObjects[i]->GetIsHidden())
	//		collisionObjects[i]->Render();
	//}

	//// Render Mario
	//this->player->Render();
}

/*
*	Clear all objects from this scene
*/
void CIntroScene::Clear()
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
void CIntroScene::Unload()
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

bool CIntroScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CIntroScene::PurgeDeletedObjects()
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
		std::remove_if(noCollisionObjects.begin(), noCollisionObjects.end(), CIntroScene::IsGameObjectDeleted),
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
		std::remove_if(collisionObjects.begin(), collisionObjects.end(), CIntroScene::IsGameObjectDeleted),
		collisionObjects.end());
}