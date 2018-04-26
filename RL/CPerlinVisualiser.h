#pragma once 

#include "CGameState.h"
#include "CWorldPlayer.h"
#include "CPerlinMap.h"

class CPerlinVisualiser : public CGameState
{
private:
	static CPerlinVisualiser m_State;
	
	CPerlinMap m_Map;
	int m_iTileSize;
	CWorldPlayer m_Player;

protected:
	CPerlinVisualiser(){}
	CPerlinVisualiser(CPerlinVisualiser const&);
	void operator=(CPerlinVisualiser const&);

public:
	void Init(CGame* pGame);
	void Cleanup();

	void Draw(CGame* pGame);
	void Update(CGame* pGame);
	void HandleInput(CGame* pGame);

	void PauseState();
	void ResumeState();

	static CPerlinVisualiser* Instance()
	{
		return &m_State;
	}
};