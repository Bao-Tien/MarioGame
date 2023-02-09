#include "BrickGolden.h"
#include "TailMario.h"
#include "PlayScene.h"
#include "Game.h"


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
	default:
		break;
	}
	
	CGameObject::GetBoundingBox(l, t, r, b);
}

string CBrickGolden::GetAnimationFromState() {
	switch (level)
	{
		case 0:
			return ID_ANI_BRICKDEBRIS;
		case 1:
			return ID_ANI_BRICKNORMAL;
		default:
			break;
	}
}

void CBrickGolden::OnOverlapWith(LPCOLLISIONEVENT e) {
	if (dynamic_cast<CTailMario*>(e->obj)) {
		if (level != 0) {
			vy = -0.18f;
		}
		level = 0;
		die_start = GetTickCount64();
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
}