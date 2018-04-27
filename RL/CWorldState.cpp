#include "stdafx.h"
#include "CWorldState.h"
#include "CMenuState.h"

#define MIN_MAP_ZOOM 2
#define MAX_MAP_ZOOM 30
//CWorldState CWorldState::m_WorldState;
 
void CWorldState::Init(CGame* pGame) {
	m_iTileSize = MIN_MAP_ZOOM;
	
	m_Map.Init(pGame);

	// init menu 
	m_MenuOverlay.Init("PAUSE");
	m_MenuOverlay.AddItem("Return to game", MENUACTION_RETURN);
	m_MenuOverlay.AddItem("Quit", MENUACTION_QUIT);

	/* Initialise player */ 
	m_Player.Initialise(pGame);

	/* Generate a world */
	if (!m_Map.Generate(pGame)) {
		printf("FAILED TO GENERATE WORLD: (Most likely world size too large)\n");
		pGame->m_StateHandler.ChangeState(CMenuState::Instance(), pGame);
	}

	/* Set spawn point */
	m_Player.SetPosition(m_Map.CalcSpawn());
}

void CWorldState::Cleanup() {}

void CWorldState::Draw(CGame* pGame) {
	/* Centre camera on player */
	pGame->m_WindowManager.MoveCamera(m_Player.GetPosition() * m_iTileSize);

	m_Map.Draw(pGame, m_iTileSize, m_Player.GetPositionI());
	m_Player.Draw(pGame, m_iTileSize);

	/* Restore default camera -> drawing in screen co-ords*/
	pGame->m_WindowManager.RestoreDefaultCamera();

	Vector2i dim = pGame->m_WindowManager.GetScreenDimensions();
	m_Map.DrawMinimap(pGame, 256, Vector2i(dim.X - 130, 130), m_Player.GetPositionI(), 10, 2);

	if (m_MenuOverlay.IsActive()) {
		m_MenuOverlay.Draw(pGame);
	}
}

void CWorldState::Update(CGame* pGame) {
	if (m_MenuOverlay.ShouldQuit())
		pGame->m_StateHandler.ChangeState(CMenuState::Instance(), pGame);
	if (m_MenuOverlay.IsActive())
		return;

	/* Player faces mouse */
	m_Player.LookAt(pGame->m_Input.GetMousePosition(), pGame);

	/* Update player */
	m_Player.Update(pGame);
}

void CWorldState::HandleInput(CGame* pGame) {
	// Menu absorbs all input if active
	if (m_MenuOverlay.IsActive()) {
		m_MenuOverlay.HandleInput(pGame);
		return;
	}

	EAction ThisAction = Action_Unknown;
	while ((ThisAction = pGame->m_Input.GetNextAction()) != Action_None) {
		switch (ThisAction)	{

		case Action_Menu_Up:
			m_iTileSize++;
			if (m_iTileSize > MAX_MAP_ZOOM)
				m_iTileSize = MAX_MAP_ZOOM;
			break;

		case Action_Menu_Down:
			m_iTileSize--;
			if (m_iTileSize < MIN_MAP_ZOOM)
				m_iTileSize = MIN_MAP_ZOOM;
			break;
		
		case Action_Menu_Toggle:
			m_MenuOverlay.Activate();
			// Menu is activated, handle menu input & return
			HandleInput(pGame);
			return;

		case Action_Reload:
			/* Regenerate world */
			if (!m_Map.Generate(pGame)) {
				printf("FAILED TO GENERATE WORLD: (Most likely world size too large)\n");
				pGame->m_StateHandler.ChangeState(CMenuState::Instance(), pGame);
			}

		default:
			break;
		}
	}

	m_Player.SetWalkForward(pGame, pGame->m_Input.ActionKeyPressed(Action_Move_Forward));
	m_Player.SetWalkBack(pGame, pGame->m_Input.ActionKeyPressed(Action_Move_Back));
	m_Player.SetLeftStrafe(pGame, pGame->m_Input.ActionKeyPressed(Action_Move_Left));
	m_Player.SetRightStrafe(pGame, pGame->m_Input.ActionKeyPressed(Action_Move_Right));
	m_Player.SetSprint(pGame, pGame->m_Input.ActionKeyPressed(Action_Sprint));
}

void CWorldState::PauseState() {
}

void CWorldState::ResumeState() {
}