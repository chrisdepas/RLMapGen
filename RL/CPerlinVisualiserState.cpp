#include "stdafx.h"
#include "CPerlinVisualiserState.h"

CPerlinVisualiserState CPerlinVisualiserState::m_State;

void CPerlinVisualiserState::Init(CGame* pGame) {
	m_iTileSize = 1;

	/* Initialise player */
	m_Player.Initialise(pGame);

	/* Generate a world */
	m_Map.Generate(pGame);

	/* Set spawn point */
	m_Player.SetPosition(m_Map.CalcSpawn());
}

void CPerlinVisualiserState::Cleanup() {}

void CPerlinVisualiserState::Draw(CGame* pGame) {
	/* Centre camera on player */
	pGame->m_WindowManager.MoveCamera(m_Player.GetPosition());

	/* Draw map */
	m_Map.Draw(pGame, m_iTileSize, m_Player.GetPositionI());

	/* Draw player */
	m_Player.Draw(pGame, m_iTileSize);
}

void CPerlinVisualiserState::Update(CGame* pGame) {
	/* Player faces mouse */
	m_Player.LookAt(pGame->m_Input.GetMousePosition(), pGame);

	/* Update player */
	m_Player.Update(pGame);
}

void CPerlinVisualiserState::HandleInput(CGame* pGame) {
	EAction ThisAction = Action_Unknown;
	while ((ThisAction = pGame->m_Input.GetNextAction()) != Action_None) {
		switch (ThisAction) {
		case Action_Move_Forward:
			break;
		case Action_Move_Back:
			break;
		case Action_Move_Left:
			break;
		case Action_Move_Right:
			break;
		default:
			break;
		}
	}

	m_Player.SetWalkForward(pGame, pGame->m_Input.ActionKeyPressed(Action_Move_Forward));
	m_Player.SetWalkBack(pGame, pGame->m_Input.ActionKeyPressed(Action_Move_Back));
	m_Player.SetLeftStrafe(pGame, pGame->m_Input.ActionKeyPressed(Action_Move_Left));
	m_Player.SetRightStrafe(pGame, pGame->m_Input.ActionKeyPressed(Action_Move_Right));
}

void CPerlinVisualiserState::PauseState() {}
void CPerlinVisualiserState::ResumeState() {}