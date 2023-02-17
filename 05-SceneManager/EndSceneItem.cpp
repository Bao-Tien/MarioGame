#include "EndSceneItem.h"
#include "Animations.h"
#include "Mario.h"

void CEndSceneItem::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
}

void CEndSceneItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	switch (level)
	{
	case 0:
		SetBoundingBoxSize(0, 0);
		break;
	case 1:
		SetBoundingBoxSize(ENDSCENEITEM_BBOX_WIDTH, ENDSCENEITEM_BBOX_HEIGHT);
		break;
	default:
		break;
	}

	CGameObject::GetBoundingBox(l, t, r, b);
}

string CEndSceneItem::GetAnimationFromState() {
	switch (level)
	{
		case 0:
			return "";
		case 1:
			return ID_ANI_ENDSCENEITEM;
		default:
			break;
	}
}

void CEndSceneItem::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (level == 0) return;
	if (e->obj) {
		level = 0;
		isBlocking = 1;
		die_start = GetTickCount64();
	}
	//DebugOut(L"level %i\n: ", level);

}

void CEndSceneItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	DebugOut(L"update %d\n", GetTickCount64() / 1000);
	if ((level == 0))
	{
		if ((GetTickCount64() - die_start > OBJECT_DIE_TIMEOUT)) {
			isDeleted = true;
		}
		return;
	}
	
}