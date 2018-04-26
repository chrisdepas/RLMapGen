#pragma once
/*
	Simple loading screen
	Draws only when progress or activity changed

	Call SetActivity() to display the current activity, along with a progress bar
	Call SetProgress() to change the progress of the current activity
*/

#define SIMPLELOADINGSCREEN_MAXACTIVITY 32
#define SIMPLELOADINGSCREEN_FONTSIZE 32
#define SIMPLELOADINGSCREEN_FONTSIZE_PREV 20
class CGame;
class CSimpleLoadingScreen
{
	char* m_szActivity[SIMPLELOADINGSCREEN_MAXACTIVITY];
	int m_iActivityCount;
	float m_fProgress;

	void Draw(CGame* pGame);
public:
	CSimpleLoadingScreen();
	~CSimpleLoadingScreen();

	void ClearActivities();
	void SetActivity(char* szActivity, CGame* pGame);
	void SetProgress(float fPercent, CGame* pGame);
};