#pragma once
#include "GameObject.h"
#include "debug.h"

#define ANI_ID "ani-raccoon-mario-idle-front"
#define VELOCITY 0.2f
#define BBOX_SIZE 16
#define ANI_WIDTH 16*3
#define ANI_HIGHT 28*3
#define CHANGE_SCENE_TIME 2000

class CMarioSelectionScene : public CGameObject
{
protected:
	ULONGLONG change_scene_start = -1;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool isStandAtGate = false;
public:
	CMarioSelectionScene(float x, float y) : CGameObject(x, y) {
		isCollidable = 1;
		isBlocking = 1;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	string GetAnimationFromState();
	void OnOverlapWith(LPCOLLISIONEVENT e);
	void KeyboardHandle(int KeyCode, EKeyType type);
};