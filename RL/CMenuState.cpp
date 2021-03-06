#include "stdafx.h"
#include "CMenuState.h"
#include "Vector.h"
#include "CGame.h"
#include "CWorldState.h"
#include "CPerlinVisualiserState.h"

CMenuState CMenuState::m_MenuState;

void CMenuState::Init(CGame* pGame) {}
void CMenuState::Cleanup() {}

void CMenuState::Draw(CGame* pGame) {
	Vector2i Centre = pGame->m_WindowManager.GetScreenCentre();
	pGame->m_Drawing.DrawTextCentred(&pGame->m_WindowManager, "RL", Centre.X, Centre.Y - MENUSTATE_FONT_SIZE / 2,
		MENUSTATE_FONT_SIZE, 255, 255, 255, 255);
	pGame->m_Drawing.DrawTextCentred(&pGame->m_WindowManager, "Press enter to play", Centre.X, Centre.Y + MENUSTATE_FONT_SIZE / 2,
		MENUSTATE_FONT_SIZE, 255, 255, 255, 255);
	pGame->m_Drawing.DrawTextCentred(&pGame->m_WindowManager, "Press ESCAPE to quit", Centre.X, Centre.Y + 3 * MENUSTATE_FONT_SIZE / 2,
		MENUSTATE_FONT_SIZE, 255, 255, 255, 255);
}
void CMenuState::Update() {}

void CMenuState::HandleInput(CGame* pGame) {
	EAction ThisAction = Action_Unknown;
	while ((ThisAction = pGame->m_Input.GetNextAction()) != Action_None) {
		switch (ThisAction)	{

		case Action_Menu_Select:
			pGame->m_StateHandler.ChangeState(CWorldState::Instance(), pGame);
			break;

		case Action_Menu_Toggle:
			pGame->Quit();
			break;

		case Action_Grab:
			pGame->m_StateHandler.ChangeState(CPerlinVisualiserState::Instance(), pGame);
			break;

		default:
			return;
		}
	}
} 

void CMenuState::PauseState() {}
void CMenuState::ResumeState() {}