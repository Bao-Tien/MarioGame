#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene2())->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE) return;
	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->KeyboardHandle(DIK_DOWN, 1);
		break;
	case DIK_S:
		mario->KeyboardHandle(DIK_S, 1);
		break;
	case DIK_1:
		mario->KeyboardHandle(DIK_1, 1);
		break;
	case DIK_2:
		mario->KeyboardHandle(DIK_2, 1);
		break;
	case DIK_3:
		mario->KeyboardHandle(DIK_3, 1);
		break;
	case DIK_0:
		mario->KeyboardHandle(DIK_0, 1);
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
	if (mario->GetState() == MARIO_STATE_DIE) return;
	switch (KeyCode)
	{
		/*case DIK_S:
			mario->SetState(MARIO_STATE_RELEASE_JUMP);
			break;*/
	case DIK_DOWN:
		mario->KeyboardHandle(DIK_DOWN, 2);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene2())->GetPlayer();

	if (mario->GetState() == MARIO_STATE_DIE) return;
	// Su kien di kem
	if (game->IsKeyDown(DIK_A))//chay nhanh
	{
		mario->KeyboardHandle(DIK_A, 3);
	}

	// Su kien rieng
	if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->KeyboardHandle(DIK_RIGHT, 3);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		mario->KeyboardHandle(DIK_LEFT, 3);
	}
	else if (game->IsKeyDown(DIK_S))
	{
		mario->KeyboardHandle(DIK_S, 3);
	}
	
}