#pragma once

#include "Animation.h"
#include "Sprite.h"
#include "Utils.h"
class CAnimation;
typedef CAnimation* LPANIMATION;

class CAnimations
{
	static CAnimations* __instance;

	unordered_map<int, LPANIMATION> animations;
	unordered_map<string, LPANIMATION> animations2;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Add(string id, LPANIMATION ani);
	LPANIMATION Get(string id);
	void LoadAnimations(string path);
	void Clear();

	static CAnimations* GetInstance();
};