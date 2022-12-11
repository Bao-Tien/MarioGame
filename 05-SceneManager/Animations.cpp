#include "Animations.h"
#include "debug.h"

CAnimations* CAnimations::__instance = NULL;

CAnimations* CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	if (animations[id] != NULL)
		DebugOut(L"[WARNING] Animation %d already exists\n", id);

	animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Animation ID %d not found\n", id);
	return ani;
}

void CAnimations::Add(string id, LPANIMATION ani)
{
	if (animations2[id] != NULL)
		DebugOut(L"[WARNING] Animation %s already exists\n", ToLPCWSTR(id));

	animations2[id] = ani;
	DebugOut(L"[INFO] Animation %s added\n", ToLPCWSTR(id));
}

LPANIMATION CAnimations::Get(string id)
{
	LPANIMATION ani = animations2[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Animation ID %s not found\n", ToLPCWSTR(id));
	return ani;
}

void CAnimations::LoadAnimations(string path)
{
	OutputDebugStringW(ToLPCWSTR(path.c_str()));
	TiXmlDocument document(path.c_str());
	if (!document.LoadFile())
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return;
	}

	TiXmlElement* root = document.RootElement();

	string gameObjectID = root->Attribute("gameObjectId");
	string textureID = root->Attribute("textureId");

	for (TiXmlElement* node = root->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
	{
		string aniId = node->Attribute("aniId");
		int frameTime;
		node->QueryIntAttribute("frameTime", &frameTime);
		CAnimation* animation = new CAnimation(aniId, frameTime);

		for (TiXmlElement* sprNode = node->FirstChildElement(); sprNode != nullptr; sprNode = sprNode->NextSiblingElement())
		{
			string spriteID;
			int detailFrameTime;
			CSprites::GetInstance()->LoadSprites(sprNode, textureID, spriteID, detailFrameTime);
			animation->Add(spriteID, detailFrameTime);
		}
		Add(aniId, animation);
		OutputDebugStringW(ToLPCWSTR(aniId));
	}
}

void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}