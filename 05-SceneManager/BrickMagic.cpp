#include "BrickMagic.h"
#include "Mario.h"
#include "Animations.h"
#include "Troopas.h"

void CBrickMagic::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
	if (status == EBox_Status::OPENED && gift == EGift_Type::COIN && isRenderedOpeningCoinEffect == false) {
		CAnimations::GetInstance()->Get(ID_ANI_COININBRICK)->Render(x, y - differenceOfCoinWithBrickQuestion);
	}
}

void CBrickMagic::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	SetBoundingBoxSize(BRICK_BBOX_WIDTH, BRICK_BBOX_HEIGHT);
	CGameObject::GetBoundingBox(l, t, r, b);
}

string CBrickMagic::GetAnimationFromState() {
	switch (status)
	{
	case EBox_Status::OPENED:
		return ID_ANI_BRICKQUESTION_EMPTY;
	case EBox_Status::NOT_OPEN:
		if (type == EBrick_Type::QUESTION) {
			return ID_ANI_BRICKQUESTION;
		}
		else {
			return ID_ANI_BRICKNORMAL;
		}
		
	default:
		break;
	}
}

void CBrickMagic::OnCollisionWith(LPCOLLISIONEVENT e) {
	//if (!e->obj->IsBlocking()) return;
	float na = e->nx;
	float nb = e->dy;

	if (((e->ny < 0 && dynamic_cast<CMario*>(e->obj)) || (e->nx != 0 && dynamic_cast<CTroopas*>(e->obj))) && status == EBox_Status::NOT_OPEN) {
		if (gift == EGift_Type::LEAF) {
			CLeaf* leaf = new CLeaf(x, y - BRICK_BBOX_HEIGHT);
			playScene->PushToDynamicObjectsFrontMap(leaf);
		}

		if (gift == EGift_Type::MUSHROOM) {
			CMushroom* mushroom = new CMushroom(x, y);
			playScene->PushToDynamicObjectsFrontMap(mushroom);
		}

		if (gift == EGift_Type::MUSHROOM_GREEN) {
			CMushroomGreen* mushroomGreen = new CMushroomGreen(x, y);
			playScene->PushToDynamicObjectsFrontMap(mushroomGreen);
		}
		
		if (gift == EGift_Type::COIN && dynamic_cast<CMario*>(e->obj)) {
			isUpedCoin = true;
			dynamic_cast<CMario*>(e->obj)->AddCoin();
			dynamic_cast<CMario*>(e->obj)->AddPoint(100);
		}

		status = EBox_Status::OPENED;
	}
}

void CBrickMagic::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (status == EBox_Status::OPENED && gift == EGift_Type::COIN && isRenderedOpeningCoinEffect == false) {
		if (isUpedCoin && differenceOfCoinWithBrickQuestion < RANGE_Y_COIN) {
			differenceOfCoinWithBrickQuestion += DIFFERENCE_OFCOIN_EACHCHANGE;
			if (differenceOfCoinWithBrickQuestion >= RANGE_Y_COIN) {
				isUpedCoin = false;
			}
		}

		if (!isUpedCoin && differenceOfCoinWithBrickQuestion > HIGH_HIDE_COIN) {
			differenceOfCoinWithBrickQuestion -= DIFFERENCE_OFCOIN_EACHCHANGE;
			if (differenceOfCoinWithBrickQuestion <= HIGH_HIDE_COIN) {
				isRenderedOpeningCoinEffect = true;
			}
		}
	}

	CGameObject::Update(dt, coObjects);
}