#pragma once

#include "GameObject.h"
#include "PlayScene.h"
#include "Leaf.h"

#define ID_ANI_BRICKQUESTION "ani-question-block"
#define ID_ANI_BRICKQUESTION_EMPTY "ani-empty-block"
#define BRICKQUESTION_BBOX_WIDTH 48
#define BRICKQUESTION_BBOX_HEIGHT 48

enum EGift_Type {
	COIN = 0,
	LEFT = 1, // LEAF
};

enum EBox_Status {
	NOT_OPEN = 0,
	OPENED = 1,
};

class CBrickQuestion : public CGameObject {
	EBox_Status status = EBox_Status::NOT_OPEN;
	EGift_Type gift = EGift_Type::COIN;
	CPlayScene* playScene;

	bool isRenderedOpeningCoinEffect = false;

public:
	CBrickQuestion(float x, float y, CPlayScene* playScene, string strGift = "") : CGameObject(x, y) {
		if (strGift == "Leaf") {
			gift = EGift_Type::LEFT;
		}
		this->playScene = playScene;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	string GetAnimationFromState();
	void OnCollisionWith(LPCOLLISIONEVENT e);
	//void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};