#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "Mario.h"
#include "Animations.h"
#include "SampleKeyEventHandler.h"

#define CURTAIN_SCROLL 3000
#define CURTAIN_Y_START 290
#define CURTAIN_Y_END -210

#define CHANGE_SCENE_TIME 2000


class CIntroScene : public CScene
{
protected:
	// A play scene has to have player, right? 
	CGameObject* player;

	vector<LPGAMEOBJECT> collisionObjects;
	vector<LPGAMEOBJECT> noCollisionObjects;

	int changeOption = -1;
	ULONGLONG curtain_start = -1;

	ULONGLONG change_scene_start = -1;

public:
	CIntroScene(string id, string filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	void KeyboardHandle(int KeyCode, EKeyType type);
};

typedef CIntroScene* LPINTROSCENE;

