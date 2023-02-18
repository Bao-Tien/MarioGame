#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "SmallMario.h"
#include "BigMario.h"
#include "Goomba.h"
#include "Animations.h"

#define SCENE_TIME 900000
#define DIE_TIME 1500

class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	CMario* player;
	int sceneTime = SCENE_TIME;
	ULONGLONG mario_die_start = -1;

	vector<LPGAMEOBJECT> staticObjects;
	vector<LPGAMEOBJECT> deathPlatformObjects;
	vector<LPGAMEOBJECT> dynamicObjectsAfterMap; //venus
	vector<LPGAMEOBJECT> dynamicObjectsFrontMap; //quai
	vector<LPGAMEOBJECT> dynamicTroopasFrontMap; //Troopas
	
public: 
	CPlayScene(int id, LPCWSTR filePath);
	CPlayScene(string id, string filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	void UpdatePlayer();
	void PushToDynamicObjectsFrontMap(LPGAMEOBJECT obj) {
		dynamicObjectsFrontMap.push_back(obj);
	}
	int GetSceneTime() { return sceneTime; }

	vector<LPGAMEOBJECT> GetStaticObjects() { return staticObjects; }
	vector<LPGAMEOBJECT> GetDynamicObjectsFrontMap() { return dynamicObjectsFrontMap; }

	void UpdateIfInCameraArea(LPGAMEOBJECT obj, DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {
		if (CGame::GetInstance()->IsInCameraArea(obj->GetPosition())) {
			obj->Update(dt, coObjects);
		}
	}

	void RenderIfInCameraArea(LPGAMEOBJECT obj) {
		if (CGame::GetInstance()->IsInCameraArea(obj->GetPosition())) {
			obj->Render();
		}
	}
};

typedef CPlayScene* LPPLAYSCENE;

