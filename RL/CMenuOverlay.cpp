#include "stdafx.h"
#include "CMenuOverlay.h"
#include "Vector.h"
#include "CGame.h"
CMenuOverlay::CMenuOverlay() {
	memset(this, 0, sizeof(CMenuOverlay));
}
CMenuOverlay::~CMenuOverlay() {

}

void CMenuOverlay::AddItem(char* szItem, EMenuAction eAction) {
	free(m_szTitle);
	m_szTitle = 0;
}

void CMenuOverlay::Draw(CGame* pGame) {
	Vector2i Centre = pGame->m_WindowManager.GetScreenCentre();
	Vector2i Dimensions = pGame->m_WindowManager.GetScreenDimensions();

	if ( m_szTitle )
		pGame->m_Drawing.DrawTextCentred(&pGame->m_WindowManager, m_szTitle, Centre.X, Centre.Y,
			SIMPLELOADINGSCREEN_FONTSIZE, 255, 255, 255, 255);

	for (int i = 0; i < m_iItemCount; i++) {
		int y = Centre.Y - (int)(((m_iItemCount - 1) - i)*SIMPLELOADINGSCREEN_FONTSIZE_PREV*1.5f);
	//	pGame->m_Drawing.DrawTextCentred(&pGame->m_WindowManager, m_szActivity[i], Centre.X, y,
	//		SIMPLELOADINGSCREEN_FONTSIZE, 255, 255, 255, 100);
	}
}
void CMenuOverlay::HandleInput(CGame* pGame) {
	
	EAction ThisAction = Action_Unknown;
	while ((ThisAction = pGame->m_Input.GetNextAction()) != Action_None)
	{
		switch (ThisAction)
		{
		case Action_Menu_Toggle:
			m_bIsActive = false;
			return;

		default:
			break;
		}
	}
}

bool CMenuOverlay::IsActive() {
	return m_bIsActive;
}

void CMenuOverlay::Activate() {
	m_bIsActive = true;
}

void CMenuOverlay::SetTitle(char* szTitle) {
	int l = strlen(szTitle);
	m_szTitle = (char*)malloc(l + 1);
	memcpy(m_szTitle, szTitle, l+1);
}