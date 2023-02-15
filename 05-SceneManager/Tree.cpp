#include "Tree.h"
#include "MarioSelectionScene.h"
#include "debug.h"

void CTree::Render()
{
	CGameObject::Render();
	//CGameObject::RenderBoundingBox();
}

void CTree::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	SetBoundingBoxSize(TREE_BBOX_WIDTH, TREE_BBOX_HEIGHT);
	CGameObject::GetBoundingBox(l, t, r, b);
}

string CTree::GetAnimationFromState() {
	return ID_ANI_TREE;
}

void CTree::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (dynamic_cast<CMarioSelectionScene*>(e->obj)) {
		DebugOut(L"collision with mario selection scene\n");
	}
}
