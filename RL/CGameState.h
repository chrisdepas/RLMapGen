#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

class CGame;
class CGameState; 

/*	A state in the game, such as main menu
Handled by CGameStateHandler */
class CGameState {

public:
	CGameState* m_pgsReturnState;
	void SetReturnState(CGameState* pReturnState);
	virtual void Init(CGame* pGame);
	virtual void Cleanup();

	virtual void Draw(CGame* pGame);
	virtual void Update(CGame* pGame);
	virtual void HandleInput(CGame* pGame);

	void ChangeState(CGameState* pState, CGame* pGame);
	virtual void PauseState();
	virtual void ResumeState();
};

#endif