#include "Tree.h"

void CTree::Render()
{
	CGameObject::Render();
	CGameObject::RenderBoundingBox();
}

void CTree::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	SetBoundingBoxSize(TREE_BBOX_WIDTH, TREE_BBOX_HEIGHT);
	CGameObject::GetBoundingBox(l, t, r, b);
}

string CTree::GetAnimationFromState() {
	return ID_ANI_TREE;
}

