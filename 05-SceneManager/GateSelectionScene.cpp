#include "GateSelectionScene.h"

void CGateSelectionScene::Render() {
	CGameObject::Render();
	CGameObject::RenderBoundingBox();
}

void CGateSelectionScene::GetBoundingBox(float& l, float& t, float& r, float& b) {
	SetBoundingBoxSize(48, 48);
	CGameObject::GetBoundingBox(l, t, r, b);
}

string CGateSelectionScene::GetAnimationFromState() {
	switch (id)
	{
	case 0:
		return "ani-gate-start";
	case 1:
		return "ani-gate-1";
	case 2:
		return "ani-gate-2";
	case 3:
		return "ani-gate-3";
	case 4:
		return "ani-gate-4";
	case 5:
		return "ani-gate-5";
	case 6:
		return "ani-gate-6";
	case 7:
		return "ani-gate-bich";
	default:
		break;
	}
}
