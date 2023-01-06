#pragma once

#include "GameObject.h"
#include "PlayScene.h"
#include "Leaf.h"
#include "Mushroom.h"
#include "MushroomGreen.h"

#define ID_ANI_BRICKNORMAL "ani-brick"
#define ID_ANI_BRICKQUESTION "ani-question-block"
#define ID_ANI_BRICKQUESTION_EMPTY "ani-empty-block"
#define BRICK_BBOX_WIDTH 48
#define BRICK_BBOX_HEIGHT 48

#define ID_ANI_COININBRICK "ani-coin-in-question-block"
#define RANGE_Y_COIN 150
#define DIFFERENCE_OFCOIN_EACHCHANGE 30
#define HIGH_HIDE_COIN 60

enum EGift_Type {
	COIN = 0,
	LEAF = 1, 
	MUSHROOM = 2,
	MUSHROOM_GREEN = 3,
};

enum EBox_Status {
	NOT_OPEN = 0,
	OPENED = 1,
};

enum EBrick_Type {
	QUESTION = 0,
	NORMAL = 1,
};

class CBrickMagic : public CGameObject {
	EBox_Status status = EBox_Status::NOT_OPEN;
	EGift_Type gift;
	EBrick_Type type;
	CPlayScene* playScene;

	bool isRenderedOpeningCoinEffect = false;

	float differenceOfCoinWithBrickQuestion = 0;
	bool isUpedCoin = false;

public:
	CBrickMagic(float x, float y, CPlayScene* playScene,string strType = "Question", string strGift = "Coin") : CGameObject(x, y) {
		SetTypeBrick(strType);
		SetGift(strGift);
		this->playScene = playScene;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	string GetAnimationFromState();
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetTypeBrick(string strType) {
		if (strType == "Question") {
			type = EBrick_Type::QUESTION;
		}
		else {
			type = EBrick_Type::NORMAL;
		}
	}
	void SetGift(string strGift) {
		if (strGift == "Leaf") {
			gift = EGift_Type::LEAF;
		}
		else if (strGift == "Mushroom") {
			gift = EGift_Type::MUSHROOM;
		}
		else if (strGift == "MushroomGreen") {
			gift = EGift_Type::MUSHROOM_GREEN;
		}
		else {
			gift = EGift_Type::COIN;
		}
	}
};