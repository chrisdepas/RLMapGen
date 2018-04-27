#ifndef __CWORLDPLAYERHUD_H__
#define __CWORLDPLAYERHUD_H__

#include "CWorldPlayer.h"

class CWorldPlayerHUD {
	CWorldPlayer* m_pLocalPlayer;

	void Initialise(CWorldPlayer* pLocal);
	void Draw(CGame* pGame);
};

#endif