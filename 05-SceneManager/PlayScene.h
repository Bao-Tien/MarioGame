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
//#include "Koopas.h"


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	CMario* player;

	vector<LPGAMEOBJECT> dynamicObjectsAfterMap;
	vector<LPGAMEOBJECT> dynamicObjectsFrontMap;
	vector<LPGAMEOBJECT> staticObjects;
	
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
};

typedef CPlayScene* LPPLAYSCENE;

