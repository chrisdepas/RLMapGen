#pragma once

class CGame;
enum EMenuAction {
	MENUACTION_RETURN,	// Return to parent menu, or game
	MENUACTION_SUBMENU,	// Move to a submenu
	MENUACTION_TOGGLE_BOOL, // Toggle a bool
	MENUACTION_QUIT,	// Quit game
};
#define MAX_MENU_ITEMS 16
class CMenuOverlay
{
	struct SMenuItem {
		char* szItem;
		EMenuAction eAction;
	};
	int m_iItemCount;
	char* m_szTitle;
	bool m_bIsActive;
	SMenuItem* m_MenuItems[MAX_MENU_ITEMS];

public:
	CMenuOverlay();
	~CMenuOverlay();

	void SetTitle(char* szTitle);
	void AddItem(char* szItem, EMenuAction eAction);
	
	void Draw(CGame* pGame);
	void HandleInput(CGame* pGame);
	
	bool IsActive();
	void Activate();
};