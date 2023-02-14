#include "MarioSelectionScene.h"
#include "Game.h"

void CMarioSelectionScene::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	SetBoundingBoxSize(48, 48);
	CGameObject::GetBoundingBox(left, top, right, bottom);
}

string CMarioSelectionScene::GetAnimationFromState() {
	return MARIOSELECTIONSCENE_ANI;
}
void CMarioSelectionScene::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);     
	CCollision::GetInstance()->Process(this, dt, coObjects);
	//reset
	vx = 0;
	vy = 0;
}

void CMarioSelectionScene::KeyboardHandle(int KeyCode, EKeyType type) {
	switch (KeyCode)
	{
	case DIK_UP:
		this->vy = -MARIOSELECTIONSCENE_V;
		break;
	case DIK_DOWN:
		this->vy = MARIOSELECTIONSCENE_V;
		break;
	case DIK_LEFT:
		this->vx = -MARIOSELECTIONSCENE_V;
		break;
	case DIK_RIGHT:
		this->vx = MARIOSELECTIONSCENE_V;
		break;
	default:
		break;
	}
}

void CMarioSelectionScene::Render()
{
	CGameObject::Render();
	CGameObject::RenderBoundingBox();
}