#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "Mario.h"
#include "Animations.h"


class CSelectionScene : public CScene
{
protected:
	// A play scene has to have player, right? 
	CGameObject* player;

	vector<LPGAMEOBJECT> collisionObjects;
	vector<LPGAMEOBJECT> noCollisionObjects;

public:
	CSelectionScene(string id, string filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

