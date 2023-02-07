#include "BrickGolden.h"
#include "TailMario.h"
#include "PlayScene.h"
#include "Game.h"


void CBrickGolden::Render()
{
	CGameObject::Render();
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
	default:
		break;
	}
	
	CGameObject::GetBoundingBox(l, t, r, b);
}

string CBrickGolden::GetAnimationFromState() {
	switch (level)
	{
		case 0:
			return "";
		case 1:
			return ID_ANI_BRICKNORMAL;
		default:
			break;
	}
}

void CBrickGolden::OnOverlapWith(LPCOLLISIONEVENT e) {
	if (dynamic_cast<CTailMario*>(e->obj)) {
		level = 0;
	}
}

void CBrickGolden::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt, coObjects);
}