#include "stdafx.h"
#include "CGameState.h"

void CGameState::SetReturnState(CGameState* pReturnState)
{
	m_pgsReturnState = pReturnState;
}
void CGameState::Init(CGame* pGame){}
void CGameState::Cleanup(){}

void CGameState::Draw(CGame* pGame){}
void CGameState::Update(CGame* pGame){}
void CGameState::HandleInput(CGame* pGame){} 

void CGameState::PauseState(){}
void CGameState::ResumeState(){}