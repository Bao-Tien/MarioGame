#pragma once

#include "Scene.h"

enum EKeyType {
	KEY_DOWN = 1,
	KEY_UP = 2,
	KEY_STATE = 3
};

class CSampleKeyHandler: public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CSampleKeyHandler(LPSCENE s) :CSceneKeyHandler(s) {};
};
