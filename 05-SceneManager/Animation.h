#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"


class CAnimation
{
	std::string id;
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	std::vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	CAnimation(string id, int defaultTime = 100) { this->id = id; this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	void Add(string spriteId, DWORD time = 0);
	void Render(float x, float y, float flipX = 1.0f, float flipY = 1.0f);
	void RenderFixed(float x, float y, float flipX = 1.0f, float flipY = 1.0f);
};

typedef CAnimation* LPANIMATION;