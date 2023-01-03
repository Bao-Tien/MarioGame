#include "Leaf.h"
#include "debug.h"

void CLeaf::Render()
{
	CGameObject::Render();
	RenderBoundingBox();
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	switch (level)
	{
	case 0:
		SetBoundingBoxSize(0, 0);
		break;
	case 1:
		SetBoundingBoxSize(LEAF_BBOX_WIDTH, LEAF_BBOX_HEIGHT);
		break;
	default:
		break;
	}

	CGameObject::GetBoundingBox(l, t, r, b);
}

string CLeaf::GetAnimationFromState() {
	switch (level)
	{
	case 0:
		return "";
	case 1:
		return ID_ANI_LEAF;
	default:
		break;
	}
}

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (e->obj) {
		level = 0;
	}
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);
	// a -> v -> s
	vy += (ay + LEAF_G) * dt;

	if (LEAF_MAX_VY != 0 && vy > LEAF_MAX_VY) vy = LEAF_MAX_VY;

	CCollision::GetInstance()->Process(this, dt, NULL);

	DebugOut(L" Leaf: x = %f, y = %f, vx = %f, vy = %f \n", x, y, vx, vy );
	//Reset
	ay = 0;
}