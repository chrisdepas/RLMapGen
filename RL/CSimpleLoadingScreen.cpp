#include "stdafx.h"
#include "CSimpleLoadingScreen.h"
#include "CGame.h"
void CSimpleLoadingScreen::ClearActivities() {
	for (int i = 0; i < m_iActivityCount; i++) {

	}
	CSimpleLoadingScreen();
}
CSimpleLoadingScreen::CSimpleLoadingScreen() 
{
	m_iActivityCount = 0;
	m_fProgress = 0.0f;
}

CSimpleLoadingScreen::~CSimpleLoadingScreen() {}

void CSimpleLoadingScreen::SetActivity(char* szActivity, CGame* pGame) {
	if (!szActivity)
		return;

	if (m_iActivityCount >= SIMPLELOADINGSCREEN_MAXACTIVITY)
		ClearActivities();

	int len = strlen(szActivity);
	m_szActivity[m_iActivityCount] = (char*)malloc(len + 1);
	memcpy(m_szActivity[m_iActivityCount], szActivity, len + 1);

	m_fProgress = 0.0f;
	m_iActivityCount++;

	Draw(pGame);
}

void CSimpleLoadingScreen::SetProgress(float fPercent, CGame* pGame) {
	m_fProgress = fPercent;
	if (m_fProgress > 1.0f) 
		m_fProgress = 1.0f;
	if (m_fProgress < 0.0f)
		m_fProgress = 0.0f;

	Draw(pGame);
}

void CSimpleLoadingScreen::Draw(CGame* pGame) {
	if (m_iActivityCount <= 0)
		return;
	pGame->m_WindowManager.Begin();
	Vector2i Centre = pGame->m_WindowManager.GetScreenCentre();
	Vector2i Dimensions = pGame->m_WindowManager.GetScreenDimensions();

	// Progress bar under activity
	if ( m_fProgress > 0.0f )
		pGame->m_Drawing.DrawOutlinedRectangleCentred(&pGame->m_WindowManager, Centre,
			(int)(Dimensions.X*m_fProgress), SIMPLELOADINGSCREEN_FONTSIZE, (char)(255 * (1.0f - m_fProgress)), (char)(255 * m_fProgress), 0, 255);
	pGame->m_Drawing.DrawTextCentred(&pGame->m_WindowManager, m_szActivity[m_iActivityCount-1], Centre.X, Centre.Y,
		SIMPLELOADINGSCREEN_FONTSIZE, 255, 255, 255, 255);

	for (int i = m_iActivityCount - 2; i >= 0; i--) {
		int y = Centre.Y - (int)(((m_iActivityCount - 1) - i)*SIMPLELOADINGSCREEN_FONTSIZE_PREV*1.5f);
		pGame->m_Drawing.DrawTextCentred(&pGame->m_WindowManager, m_szActivity[i], Centre.X, y,
			SIMPLELOADINGSCREEN_FONTSIZE, 255, 255, 255, 100);
	}

	pGame->m_WindowManager.Present();
}