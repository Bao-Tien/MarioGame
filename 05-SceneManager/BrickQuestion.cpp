#include "BrickQuestion.h"
#include "Mario.h"

void CBrickQuestion::Render()
{
	CGameObject::Render();
	RenderBoundingBox();

	if (status == EBox_Status::OPENED && gift == EGift_Type::COIN && isRenderedOpeningCoinEffect == false) {
		// y: 910, 900, 890, 880, 870, 860, 870, 880, 890, 900, 910, 920, 930

		// 930 isRenderedOpeningCoinEffect = true
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
		if (gift == EGift_Type::LEFT) {
			CLeaf* leaf = new CLeaf(x, y - BRICKQUESTION_BBOX_HEIGHT);
			playScene->PushToDynamicObjectsFrontMap(leaf);
		}

		status = EBox_Status::OPENED;
	}
}

