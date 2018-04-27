#ifndef __CPERLINVISUALISERSTATE_H__
#define __CPERLINVISUALISERSTATE_H__

#include "CGameState.h"
#include "CWorldPlayer.h"
#include "CPerlinMap.h"

class CPerlinVisualiserState : public CGameState {

private:
	static CPerlinVisualiserState m_State;
	
	CPerlinMap m_Map;
	int m_iTileSize;
	CWorldPlayer m_Player;

protected:
	CPerlinVisualiserState(){}
	CPerlinVisualiserState(CPerlinVisualiserState const&);
	void operator=(CPerlinVisualiserState const&);

public:
	void Init(CGame* pGame);
	void Cleanup();

	void Draw(CGame* pGame);
	void Update(CGame* pGame);
	void HandleInput(CGame* pGame);

	void PauseState();
	void ResumeState();

	static CPerlinVisualiserState* Instance() {
		return &m_State;
	}
};

#endif