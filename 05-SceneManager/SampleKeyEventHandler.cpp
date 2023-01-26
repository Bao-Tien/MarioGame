#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene2())->GetPlayer();
	if (mario->GetState() == EMario_State::DIE) return;
	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->KeyboardHandle(DIK_DOWN, EKeyType::KEY_DOWN);
		break;
	case DIK_1:
		mario->KeyboardHandle(DIK_1, EKeyType::KEY_DOWN);
		break;
	case DIK_S:
		mario->KeyboardHandle(DIK_S, EKeyType::KEY_DOWN);
		break;
	case DIK_2:
		mario->KeyboardHandle(DIK_2, EKeyType::KEY_DOWN);
		break;
	case DIK_3:
		mario->KeyboardHandle(DIK_3, EKeyType::KEY_DOWN);
		break;
	case DIK_0:
		mario->KeyboardHandle(DIK_0, EKeyType::KEY_DOWN);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene2())->GetPlayer();
	if (mario->GetState() == EMario_State::DIE) return;
	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->KeyboardHandle(DIK_DOWN, EKeyType::KEY_UP);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene2())->GetPlayer();

	if (mario->GetState() == EMario_State::DIE) return;
	// Su kien di kem
	if (game->IsKeyDown(DIK_A))//chay nhanh
	{
		mario->KeyboardHandle(DIK_A, EKeyType::KEY_STATE);
	}

	// Su kien rieng
	if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->KeyboardHandle(DIK_RIGHT, EKeyType::KEY_STATE);
	}
	if (game->IsKeyDown(DIK_LEFT))
	{
		mario->KeyboardHandle(DIK_LEFT, EKeyType::KEY_STATE);
	}
	if (game->IsKeyDown(DIK_S))
	{
		mario->KeyboardHandle(DIK_S, EKeyType::KEY_STATE);
	}
}