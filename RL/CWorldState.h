#ifndef __CWORLDSTATE_H__
#define __CWORLDSTATE_H__

#include "CGameState.h"
#include "CWorldPlayer.h"
#include "CWorldMap.h"
#include "CMenuOverlay.h"

class CWorldState : public CGameState {

private:
	CWorldMap m_Map;
	int m_iTileSize;
	CWorldPlayer m_Player;
	CMenuOverlay m_MenuOverlay;

protected:
	CWorldState(){}
	CWorldState(CWorldState const&);
	void operator=(CWorldState const&);

public:
	void Init(CGame* pGame);
	void Cleanup();

	void Draw(CGame* pGame);
	void Update(CGame* pGame);
	void HandleInput(CGame* pGame);
	 
	void PauseState();
	void ResumeState(); 

	static CWorldState* Instance() {
		static CWorldState WorldState;
		return &WorldState;
	}
};

#endif