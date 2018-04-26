#pragma once

#include "CWorldPlayer.h"
class CWorldPlayerHUD
{
	CWorldPlayer* m_pLocalPlayer;

	void Initialise(CWorldPlayer* pLocal);
	void Draw(CGame* pGame);
};