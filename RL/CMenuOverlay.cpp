#include "stdafx.h"
#include "CMenuOverlay.h"
#include "Vector.h"
#include "CGame.h"

#define MENUOVERLAY_FONTSIZE 24

CMenuOverlay::CMenuOverlay() {
	m_bIsActive = false;
	m_bShouldQuit = false;
	m_iItemCount = 0;
	m_szTitle = 0;
	m_iSelectedIndex = 0;
}

CMenuOverlay::~CMenuOverlay() {
	free(m_szTitle);
	m_szTitle = 0;
}

void CMenuOverlay::Clear() {
	for (int i = 0; i < m_iItemCount; i++) {
		free(m_MenuItems[i]->szItem);
		free(m_MenuItems[i]);
	}
	m_iItemCount = 0;
	m_bShouldQuit = false;
	m_iSelectedIndex = 0;
	m_szTitle = 0;
	m_bIsActive = false;
}
void CMenuOverlay::Init(char* szTitle) {
	Clear();
	SetTitle(szTitle);
}

bool CMenuOverlay::ShouldQuit() {
	return m_bShouldQuit;
}
void CMenuOverlay::Quit() {
	m_bShouldQuit = true;
}

void CMenuOverlay::Return() {
	m_bIsActive = false;
}

void CMenuOverlay::HandleSelect() {
	int action = m_MenuItems[m_iSelectedIndex]->eAction;

	switch (action) {
	case MENUACTION_RETURN:
		Return(); break; 
	case MENUACTION_QUIT:
		Quit(); break;
	default:
		printf("[CMENUOVERLAY] Unhandled action %i\n", action);
	}
}

void CMenuOverlay::AddItem(char* szItem, EMenuAction eAction) {
	if (m_iItemCount >= MENUOVERLAY_MAX_ITEMS)
		return;

	m_MenuItems[m_iItemCount] = (SMenuItem*)malloc(sizeof(SMenuItem));
	m_MenuItems[m_iItemCount]->eAction = eAction;

	int l = strlen(szItem);
	m_MenuItems[m_iItemCount]->szItem = (char*)malloc(l + 1);
	memcpy(m_MenuItems[m_iItemCount]->szItem, szItem, l + 1);

	m_iItemCount++;
}

void CMenuOverlay::Draw(CGame* pGame) {
	Vector2i Centre = pGame->m_WindowManager.GetScreenCentre();
	Vector2i Dimensions = pGame->m_WindowManager.GetScreenDimensions();
	Vector2i Draw = Vector2i(Centre.X, Centre.Y - ((MENUOVERLAY_FONTSIZE + 2) * m_iItemCount));

	pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager, Centre, 200, Dimensions.Y, 0, 0, 0, 255);

	if (m_szTitle) {
		pGame->m_Drawing.DrawTextCentred(&pGame->m_WindowManager, m_szTitle, Draw.X, Draw.Y,
			MENUOVERLAY_FONTSIZE + 8, 255, 255, 255, 255);
	}
	Draw.Y += MENUOVERLAY_FONTSIZE + 2;

	for (int i = 0; i < m_iItemCount; i++) {
		pGame->m_Drawing.DrawTextCentred(&pGame->m_WindowManager, m_MenuItems[i]->szItem, Draw.X, Draw.Y,
			MENUOVERLAY_FONTSIZE, (m_iSelectedIndex == i) ? 0 : 255, 255, 255, 255);
		Draw.Y += MENUOVERLAY_FONTSIZE + 2;
	}
}
void CMenuOverlay::HandleInput(CGame* pGame) {
	
	EAction ThisAction = Action_Unknown;
	while ((ThisAction = pGame->m_Input.GetNextAction()) != Action_None) {
		switch (ThisAction)	{

		case Action_Menu_Toggle:
			m_bIsActive = false;
			return;

		case Action_Menu_Down:
			m_iSelectedIndex++;
			if (m_iSelectedIndex >= m_iItemCount)
				m_iSelectedIndex = m_iItemCount - 1;
			break;

		case Action_Menu_Up:
			m_iSelectedIndex--;
			if (m_iSelectedIndex <= 0)
				m_iSelectedIndex = 0;
			break;
		
		case Action_Menu_Select:
			HandleSelect();
			break;

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