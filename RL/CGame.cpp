#include "stdafx.h"
#include "CGame.h"
#include "CMenuState.h"

CGame::CGame() {
}

CGame::~CGame() {
}

void CGame::Initialise() {
	m_bInitialised = true; 

	/* Load settings & save to disk */
	m_Settings.LoadFromFile(SETTINGS_FILE_PATH, &m_IniReader);
	m_Settings.SaveToFile(SETTINGS_FILE_PATH, &m_IniWriter);

	/* Create a window and initialise drawing */
	m_WindowManager.Create(&m_Settings, "Game");
	m_Drawing.Initialise();

	/* Set initial state to MenuState */
	m_StateHandler.ChangeState(CMenuState::Instance(), this);

	m_Random.Init();

	/* Set up input defaults */
	m_Input.Initialise();
}

bool CGame::ShouldQuit() {
	return m_WindowManager.WindowClosed();
}

void CGame::Draw() {
	m_WindowManager.Begin();
	m_StateHandler.Draw(this);
	m_WindowManager.Present();
}

void CGame::HandleInput() {
	m_StateHandler.HandleInput(this);
}

void CGame::Step() {
	m_WindowManager.HandleEvents(&m_Input);
	m_StateHandler.Update(this);
}

void CGame::Quit() {
	exit(0);
}