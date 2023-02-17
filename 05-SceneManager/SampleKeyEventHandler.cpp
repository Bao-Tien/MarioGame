#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"
#include "MarioSelectionScene.h"
#include "Mario.h"
#include "PlayScene.h"
#include "SelectionScene.h"
#include "IntroScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	LPSCENE currentScene = CGame::GetInstance()->GetCurrentScene2();
	if (dynamic_cast<CPlayScene*>(currentScene)) {
		CGameObject* player = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene2())->GetPlayer();
		if (dynamic_cast<CMario*>(player)) {
			CMario* mario = (CMario*)(player);
			if (mario->GetState() == EMario_State::DIE) return;
			switch (KeyCode)
			{
			case DIK_DOWN:
				mario->KeyboardHandle(DIK_DOWN, EKeyType::KEY_DOWN);
				break;
			case DIK_S:
				mario->KeyboardHandle(DIK_S, EKeyType::KEY_DOWN);
				break;
			case DIK_A:
				mario->KeyboardHandle(DIK_A, EKeyType::KEY_DOWN);
				break;
			case DIK_1:
				mario->KeyboardHandle(DIK_1, EKeyType::KEY_DOWN);
				break;
			case DIK_2:
				mario->KeyboardHandle(DIK_2, EKeyType::KEY_DOWN);
				break;
			case DIK_3:
				mario->KeyboardHandle(DIK_3, EKeyType::KEY_DOWN);
				break;
			case DIK_4:
				mario->KeyboardHandle(DIK_4, EKeyType::KEY_DOWN);
				break;
			case DIK_5:
				mario->KeyboardHandle(DIK_5, EKeyType::KEY_DOWN);
				break;
			case DIK_6:
				mario->KeyboardHandle(DIK_6, EKeyType::KEY_DOWN);
				break;
			case DIK_7:
				mario->KeyboardHandle(DIK_7, EKeyType::KEY_DOWN);
				break;
			case DIK_8:
				mario->KeyboardHandle(DIK_8, EKeyType::KEY_DOWN);
				break;
			case DIK_9:
				mario->KeyboardHandle(DIK_9, EKeyType::KEY_DOWN);
				break;
			case DIK_0:
				mario->KeyboardHandle(DIK_0, EKeyType::KEY_DOWN);
				break;
			case DIK_R: // reset
				//Reload();
				break;
			}
		}
	}
	if (dynamic_cast<CSelectionScene*>(currentScene)) {
		CGameObject* player = ((CSelectionScene*)CGame::GetInstance()->GetCurrentScene2())->GetPlayer();
		if (dynamic_cast<CMarioSelectionScene*>(player)) {
			CMarioSelectionScene* mario = (CMarioSelectionScene*)(player);
			switch (KeyCode)
			{
			case DIK_X:
				mario->KeyboardHandle(DIK_X, EKeyType::KEY_DOWN);
				break;
			default:
				break;
			}
		}
	}
	if (dynamic_cast<CIntroScene*>(currentScene)) {
		CIntroScene* scene = (CIntroScene*)(currentScene);
		switch (KeyCode)
		{
		case DIK_S:
			scene->KeyboardHandle(DIK_S, EKeyType::KEY_DOWN);
			break;
		case DIK_Q:
			scene->KeyboardHandle(DIK_Q, EKeyType::KEY_DOWN);
			break;
		default:
			break;
		}
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	CGameObject* player = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene2())->GetPlayer();
	if (dynamic_cast<CMario*>(player)) {
		CMario* mario = (CMario*)(player);
		if (mario->GetState() == EMario_State::DIE) return;
		switch (KeyCode)
		{
		case DIK_DOWN:
			mario->KeyboardHandle(DIK_DOWN, EKeyType::KEY_UP);
			break;
		}
	}
	
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CGameObject* player = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene2())->GetPlayer();
	if (dynamic_cast<CMario*>(player)) {
		CMario* mario = (CMario*)(player);
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
		/*if (game->IsKeyDown(DIK_S))
		{
			mario->KeyboardHandle(DIK_S, EKeyType::KEY_STATE);
		}*/
	}
	if (dynamic_cast<CMarioSelectionScene*>(player)) {
		CMarioSelectionScene* mario = (CMarioSelectionScene*)(player);
		if (game->IsKeyDown(DIK_UP))
		{
			mario->KeyboardHandle(DIK_UP, EKeyType::KEY_STATE);
		}
		if (game->IsKeyDown(DIK_DOWN))
		{
			mario->KeyboardHandle(DIK_DOWN, EKeyType::KEY_STATE);
		}
		if (game->IsKeyDown(DIK_RIGHT))
		{
			mario->KeyboardHandle(DIK_RIGHT, EKeyType::KEY_STATE);
		}
		if (game->IsKeyDown(DIK_LEFT))
		{
			mario->KeyboardHandle(DIK_LEFT, EKeyType::KEY_STATE);
		}
	}
}