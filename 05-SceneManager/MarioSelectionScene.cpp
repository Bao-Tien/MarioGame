#include "MarioSelectionScene.h"
#include "Game.h"
#include "GateSelectionScene.h"


void CMarioSelectionScene::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	SetBoundingBoxSize(BBOX_SIZE, BBOX_SIZE);
	CGameObject::GetBoundingBox(left, top, right, bottom);
}

string CMarioSelectionScene::GetAnimationFromState() {
	return ANI_ID;
}
void CMarioSelectionScene::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	isStandAtGate = false;
	if (change_scene_start != -1 && GetTickCount64() - change_scene_start > CHANGE_SCENE_TIME) {
		if (CGame::GetInstance()->GetCurrentScene2()->GetId() == "selectionScene") {
			CGame::GetInstance()->InitiateSwitchScene("world1-1");
			CGame::GetInstance()->SwitchScene2();
		}
		return;
	}
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
		this->vy = -VELOCITY;
		break;
	case DIK_DOWN:
		this->vy = VELOCITY;
		break;
	case DIK_LEFT:
		this->vx = -VELOCITY;
		break;
	case DIK_RIGHT:
		this->vx = VELOCITY;
		break;
	case DIK_X:
		if (isStandAtGate == true) {
			if (change_scene_start == -1) {
				change_scene_start = GetTickCount64();
				CGame::GetInstance()->StartCloseEffect();
			}
		}
		break;
	default:
		break;
	}
}

void CMarioSelectionScene::Render()
{
	//CGameObject::Render();
	std::string animationId = GetAnimationFromState();
	if (animationId.length() > 0) {
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(animationId)->Render(x, y + BBOX_SIZE / 2 - ANI_HIGHT / 2, flipX, flipY);;
	}

	CGameObject::RenderBoundingBox();
}

void CMarioSelectionScene::OnOverlapWith(LPCOLLISIONEVENT e) {
	if (e->obj) {
		if (dynamic_cast<CGateSelectionScene*>(e->obj)) {
			CGateSelectionScene* gate = (CGateSelectionScene*)(e->obj);
			if (gate->GetId() != 0 && gate->GetId() != 7) {
				isStandAtGate = true;
			}
		}
	}
}