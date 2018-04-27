#ifndef __CGAMESTATEHANDLER_H__
#define __CGAMESTATEHANDLER_H__

class CGameState;
class CGame;

/*	Handle game states and allow switching
TODO: Possibly make states have a stack,
So you can return etc */
class CGameStateHandler {
	/* Current game state */
	CGameState* m_State;

public:
	CGameStateHandler();
	~CGameStateHandler();

	/* Returns the current game state*/
	CGameState* GetCurrentState();

	/* Changes to specified state */
	void ChangeState(CGameState* State, CGame* pGame);

	void Draw(CGame* pGame);
	void Update(CGame* pGame);
	void HandleInput(CGame* pGame);
};

#endif