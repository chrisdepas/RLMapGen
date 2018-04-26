#pragma once
#include "CGameStateHandler.h"
#include "CWindowManager.h"
#include "CDrawingManager.h"
#include "CTextureManager.h"
#include "CTime.h"
#include "CRandom.h"
#include "CSimpleLoadingScreen.h"
#define SETTINGS_FILE_PATH ".\\Settings.ini"
class CGame
{
	bool				m_bInitialised;
public:
	CSettings			m_Settings;
	CWindowManager		m_WindowManager;
	CGameStateHandler	m_StateHandler;
	CDrawingManager		m_Drawing;
	CInput				m_Input;
	CIniReader			m_IniReader;
	CIniWriter			m_IniWriter;
	CRandom				m_Random;
	CTextureManager		m_TextureManager;
	CTime				m_Time;
	CSimpleLoadingScreen m_Loading;
//	CSoundManager		m_SoundManager;

	CGame();
	~CGame();
	void Initialise();
	bool ShouldQuit();
	void Draw();
	void HandleInput();
	void Step();
};