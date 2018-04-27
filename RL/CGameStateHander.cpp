#include "stdafx.h"
#include "CGameStateHandler.h"
#include "CGameState.h"

CGameStateHandler::CGameStateHandler() {
	m_State = 0;
}
CGameStateHandler::~CGameStateHandler() {
	if (m_State) 
		m_State->Cleanup();
}
CGameState* CGameStateHandler::GetCurrentState() {
	return m_State;
}
void CGameStateHandler::ChangeState(CGameState* pState, CGame* pGame) {
	if (pState) {
		m_State = pState;
		m_State->Init(pGame);
	}
}
void CGameStateHandler::Draw(CGame* pGame) {
	if (!m_State)
		return;
	m_State->Draw(pGame);
}
void CGameStateHandler::Update(CGame* pGame) {
	if (!m_State)
		return;
	m_State->Update(pGame);
}
void CGameStateHandler::HandleInput(CGame* pGame) {
	if (!m_State)
		return;
	m_State->HandleInput(pGame);
}