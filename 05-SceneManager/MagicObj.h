#pragma once
#include "GameObject.h"
class CMagicObj : public CGameObject
{
protected:
	int level;
public:
	CMagicObj(float x, float y) : CGameObject(x, y) {}
	int GetLevel() { return level; }
};

