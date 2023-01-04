#include "BrickQuestion.h"
#include "Mario.h"
#include "Animations.h"

void CBrickQuestion::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
	if (status == EBox_Status::OPENED && gift == EGift_Type::COIN && isRenderedOpeningCoinEffect == false) {
		CAnimations::GetInstance()->Get(ID_ANI_COININBRICK)->Render(x, y - differenceOfCoinWithBrickQuestion);
	}
}

void CBrickQuestion::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	SetBoundingBoxSize(BRICKQUESTION_BBOX_WIDTH, BRICKQUESTION_BBOX_HEIGHT);
	CGameObject::GetBoundingBox(l, t, r, b);
}

string CBrickQuestion::GetAnimationFromState() {
	switch (status)
	{
	case EBox_Status::OPENED:
		return ID_ANI_BRICKQUESTION_EMPTY;
	case EBox_Status::NOT_OPEN:
		return ID_ANI_BRICKQUESTION;
	default:
		break;
	}
}

void CBrickQuestion::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (!e->obj->IsBlocking()) return;

	if (e->ny < 0 && dynamic_cast<CMario*>(e->obj) && status == EBox_Status::NOT_OPEN) {
		if (gift == EGift_Type::LEAF) {
			CLeaf* leaf = new CLeaf(x, y - BRICKQUESTION_BBOX_HEIGHT);
			playScene->PushToDynamicObjectsFrontMap(leaf);
		}

		if (gift == EGift_Type::MUSHROOM) {
			CMushroom* mushroom = new CMushroom(x, y);
			playScene->PushToDynamicObjectsFrontMap(mushroom);
		}
		
		if (gift == EGift_Type::COIN) {
			isUpedCoin = true;
		}

		status = EBox_Status::OPENED;
	}
}

void CBrickQuestion::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
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