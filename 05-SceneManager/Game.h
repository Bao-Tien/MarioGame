#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <unordered_map>

using namespace std;

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Texture.h"
#include "KeyEventHandler.h"
#include "Scene.h"
#include "Animations.h"

#define MAX_FRAME_RATE 100
#define KEYBOARD_BUFFER_SIZE 1024
#define KEYBOARD_STATE_SIZE 256

#define BEGIN_EFFECT_TIME 3000
#define CLOSE_EFFECT_TIME 3000

/*
	Our simple game framework
*/
class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	int backBufferWidth = 0;					// Backbuffer width & height, will be set during Direct3D initialization
	int backBufferHeight = 0;

	ID3D10Device* pD3DDevice = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	ID3D10RenderTargetView* pRenderTargetView = NULL;
	ID3D10BlendState* pBlendStateAlpha = NULL;			// To store alpha blending state

	LPD3DX10SPRITE spriteObject;						// Sprite handling object, BIG MYSTERY: it has to be in this place OR will lead to access violation in D3D11.dll ????

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[KEYBOARD_STATE_SIZE];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	HINSTANCE hInstance;

	ID3D10SamplerState* pPointSamplerState;

	unordered_map<string, LPSCENE> scenes2;
	string current_scene2;
	string next_scene2 = "- 1";

	ULONGLONG begin_effect_start = -1;
	ULONGLONG close_effect_start = -1;

	void RenderBeginEffect();
	void RenderCloseEffect();

public:
	// Init DirectX, Sprite Handler
	void Init(HWND hWnd, HINSTANCE hInstance);

	//
	// Draw a portion or ALL the texture at position (x,y) on the screen. (x,y) is at the CENTER of the image
	// rect : if NULL, the whole texture will be drawn
	//        if NOT NULL, only draw that portion of the texture 
	void Draw(float x, float y, LPTEXTURE tex, RECT* rect = NULL, float alpha = 1.0f, int sprite_width = 0, int sprite_height = 0);

	void Draw(float x, float y, LPTEXTURE tex, int l, int t, int r, int b, float alpha = 1.0f, int sprite_width = 0, int sprite_height = 0)
	{
		RECT rect;
		rect.left = l;
		rect.top = t;
		rect.right = r;
		rect.bottom = b;
		if (this->IsInWindowArea(D3DXVECTOR2(x,y))) {
			this->Draw(x, y, tex, &rect, alpha, sprite_width, sprite_height);
		}
		
	}

	LPTEXTURE LoadTexture(LPCWSTR texturePath);

	// Keyboard related functions 
	void InitKeyboard();
	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }


	ID3D10Device* GetDirect3DDevice() { return this->pD3DDevice; }
	IDXGISwapChain* GetSwapChain() { return this->pSwapChain; }
	ID3D10RenderTargetView* GetRenderTargetView() { return this->pRenderTargetView; }

	ID3DX10Sprite* GetSpriteHandler() { return this->spriteObject; }

	ID3D10BlendState* GetAlphaBlending() { return pBlendStateAlpha; };

	int GetBackBufferWidth() { return backBufferWidth; }
	int GetBackBufferHeight() { return backBufferHeight; }

	static CGame* GetInstance();

	void SetPointSamplerState();

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
	void GetCamPos(float& x, float& y) { x = cam_x; y = cam_y; }
	D3DXVECTOR2 GetCamPos() {
		return D3DXVECTOR2(cam_x, cam_y);
	}

	bool IsInCameraArea(D3DXVECTOR2 objPos) {
		D3DXVECTOR2 paddingArea = D3DXVECTOR2(backBufferWidth * 0.5f, backBufferHeight * 0.5f);
		bool conditionX = (objPos.x >= cam_x - paddingArea.x && objPos.x <= cam_x + backBufferWidth + paddingArea.x);
		bool conditionY = (objPos.y >= cam_y - paddingArea.y && objPos.y <= cam_y + backBufferHeight + paddingArea.y);
		return conditionX && conditionY;
	}

	bool IsInWindowArea(D3DXVECTOR2 objPos) {
		D3DXVECTOR2 paddingArea = D3DXVECTOR2(backBufferWidth * 0.5f, backBufferHeight * 0.5f);
		bool conditionX = (objPos.x >= - paddingArea.x && objPos.x <= backBufferWidth + paddingArea.x);
		bool conditionY = (objPos.y >= - paddingArea.y && objPos.y <= backBufferHeight + paddingArea.y);
		return conditionX && conditionY;
	}

	LPSCENE GetCurrentScene2() { return scenes2[current_scene2]; }

	void Load(string gameFile);
	void SwitchScene2();
	void InitiateSwitchScene(string scene_id);


	void RenderEffect();
	
	void StartBeginEffect();
	void StartCloseEffect();

	~CGame();
};
typedef CGame* LPGAME;

