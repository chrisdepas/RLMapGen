#ifndef __CMENUOVERLAY_H__
#define __CMENUOVERLAY_H__

#define MENUOVERLAY_MAX_ITEMS 16

class CGame;

enum EMenuAction {
	MENUACTION_RETURN,	// Return to parent menu, or game
	MENUACTION_SUBMENU,	// Move to a submenu
	MENUACTION_TOGGLE_BOOL, // Toggle a bool
	MENUACTION_QUIT,	// Quit game
};

class CMenuOverlay {

	struct SMenuItem {
		char* szItem;
		EMenuAction eAction;
	};

	SMenuItem* m_MenuItems[MENUOVERLAY_MAX_ITEMS];

	int m_iSelectedIndex;
	int m_iItemCount;
	char* m_szTitle;
	bool m_bIsActive;
	bool m_bShouldQuit;
	
	void HandleSelect();
	void SetTitle(char* szTitle);
	void Clear();

public:
	CMenuOverlay();
	~CMenuOverlay();

	void Init(char* szTitle);
	void AddItem(char* szItem, EMenuAction eAction);
	
	void Draw(CGame* pGame);
	void HandleInput(CGame* pGame);
	
	bool IsActive();
	void Activate();
	bool ShouldQuit();

	void Return();
	void Quit();
};

#endif