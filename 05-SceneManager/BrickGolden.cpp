#include "BrickGolden.h"
#include "TailMario.h"
#include "PlayScene.h"
#include "Game.h"
#include "Troopas.h"
#include "SwitchBlock.h"


void CBrickGolden::Render()
{
	std::string animationId = GetAnimationFromState();
	if (animationId.length() > 0) {
		CAnimations* animations = CAnimations::GetInstance();
		if (level == 0) {
			animations->Get(animationId)->Render(x + (r * -1), y + (r * -1), flipX, flipY);
			animations->Get(animationId)->Render(x + (r * 1), y + (r * -1), flipX, flipY);
			animations->Get(animationId)->Render(x + (r * 1), y + (r * 1), flipX, flipY);
			animations->Get(animationId)->Render(x + (r * -1), y + (r * 1), flipX, flipY);
		}
		else {
			animations->Get(animationId)->Render(x, y, flipX, flipY);
		}
	}
	RenderBoundingBox();
}

void CBrickGolden::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	switch (level)
	{
	case 0:
		SetBoundingBoxSize(0, 0);
		break;
	case 1:
		SetBoundingBoxSize(BRICK_BBOX_WIDTH, BRICK_BBOX_HEIGHT);
		break;
	case 2:
		SetBoundingBoxSize(BRICK_BBOX_WIDTH, BRICK_BBOX_WIDTH);
		break;
	default:
		break;
	}
	
	CGameObject::GetBoundingBox(l, t, r, b);
}

string CBrickGolden::GetAnimationFromState() {
	switch (level)
	{
		case 0:
			if (isSWitched) {
				return "";
			}
			return ID_ANI_BRICKDEBRIS;
		case 1:
			return ID_ANI_BRICKNORMAL;
		case 2:
			return ID_ANI_COIN;
		default:
			break;
	}
}

void CBrickGolden::OnOverlapWith(LPCOLLISIONEVENT e) {
	if (dynamic_cast<CTailMario*>(e->obj) && level == 1) {
		if (level != 0) {
			vy = -0.18f;
		}
		level = 0;
		die_start = GetTickCount64();
	}
	if (level == 2 && dynamic_cast<CMario*>(e->obj)) {
		level = 0;
		die_start = GetTickCount64();
	}
}

void CBrickGolden::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (e->obj) {
		if (dynamic_cast<CTroopas*>(e->obj) && dynamic_cast<CTroopas*>(e->obj)->GetLevel() == 1 && level == 1) {
			level = 0;
			die_start = GetTickCount64();
		}
		/*if (dynamic_cast<CMario*>(e->obj) && level == 2) {
			level = 0;
			die_start = GetTickCount64();
		}*/
	}
}

void CBrickGolden::OnChangeLevel() {
	switch (level)
	{
		case 0:
			isCollidable = 0;
			isBlocking = 1;
			break;
		case 1:
			isCollidable = 0;
			isBlocking = 1;
			break;
		case 2:
			switch_start = GetTickCount64();
			isCollidable = 1;
			isBlocking = 0;
			break;
		default:
			break;
	}
}

void CBrickGolden::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt, coObjects);
	if (level == 0) {
		if (r < R_MAX) {
			r += 10;
		}
		vy += 0.03f;
		y += vy * dt;

		if ((GetTickCount64() - die_start > BRICKGOLDEN_DIE_TIMEOUT)) {
			isDeleted = true;
		}
	}

	//chuyen brick -> coin
	if (level == 1 && isSWitched == false) {
		CScene* currentScene = CGame::GetInstance()->GetCurrentScene2();
		if (dynamic_cast<CPlayScene*>(currentScene)) {
			vector<LPGAMEOBJECT> dynamicObjectsFrontMap = ((CPlayScene*)currentScene)->GetDynamicObjectsFrontMap();
			for (int i = 0; i < dynamicObjectsFrontMap.size(); i++) {
				if (dynamic_cast<CSwitchBlock*>(dynamicObjectsFrontMap[i])
					&& dynamic_cast<CSwitchBlock*>(dynamicObjectsFrontMap[i])->GetLevel() == 0) {
					level = 2;
					OnChangeLevel();
					isSWitched = true;
				}
			}
		}
	}
	if (level == 2) {
		if ((GetTickCount64() - switch_start > SWITCH_TIMEOUT)) {
			level = 1;
			OnChangeLevel();
		}
	}
}