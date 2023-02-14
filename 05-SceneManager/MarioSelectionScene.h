#pragma once
#include "GameObject.h"

#define MARIOSELECTIONSCENE_ANI "ani-raccoon-mario-idle-front"
#define MARIOSELECTIONSCENE_V 0.2f

class CMarioSelectionScene : public CGameObject
{
protected:
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
public:
	CMarioSelectionScene(float x, float y) : CGameObject(x, y) {
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	string GetAnimationFromState();
	void OnOverlapWith(LPCOLLISIONEVENT e) override {};
	void KeyboardHandle(int KeyCode, EKeyType type);
};