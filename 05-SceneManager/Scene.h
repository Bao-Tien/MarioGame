#pragma once

#include "KeyEventHandler.h"
#include <string>
#include "GameMap.h"

using namespace std;


/*
*  Abstract class for a game scene
*/
class CScene
{
protected:
	LPKEYEVENTHANDLER key_handler;
	int id;
	LPCWSTR sceneFilePath;
	string id2;
	string sceneFilePath2;
	shared_ptr<CGameMap> map;


	void CleanObjList(vector<LPGAMEOBJECT>& objList) {
		for (int i = 0; i < objList.size(); i++)
			if (objList[i] != nullptr)
				delete objList[i];

		objList.clear();
	}

public: 
	CScene(int id, LPCWSTR filePath)
	{
		this->id = id;
		this->sceneFilePath = filePath;
		this->key_handler = NULL;
	}
	CScene(string id, string filePath)
	{
		this->id2 = id;
		this->sceneFilePath2 = filePath;
		this->key_handler = NULL;
	}


	LPKEYEVENTHANDLER GetKeyEventHandler() { return key_handler; }
	string GetId() { return this->id2; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0; 
	D3DXVECTOR2 GetMapSize() {
		return this->map->GetMapSize();
	}
};
typedef CScene * LPSCENE;


class CSceneKeyHandler : public CKeyEventHandler
{
protected: 
	CScene * scence; 

public:
	virtual void KeyState(BYTE *states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CSceneKeyHandler(LPSCENE s) :CKeyEventHandler() { scence = s; }
};

typedef CSceneKeyHandler* LPSCENEKEYHANDLER;