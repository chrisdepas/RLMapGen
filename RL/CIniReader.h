#ifndef __CINIREADER_H__
#define __CINIREADER_H__

#include <vector>
#include "CIniItem.h"

#define INIREADER_MAX_SECTIONS 64

class CIniReader { 

	struct CIniSection {
		char* m_szSectionName;
		std::vector< CIniItem* > m_SectionItems;
		bool MatchesSection(char* szSection);
	};

	std::vector<CIniItem*> m_DefaultItems;
	std::vector<CIniSection*> m_Sections;
	void ClearItems(); 

public:
	CIniReader();
	~CIniReader();

	bool	ParseFile(char* FileName);

	CIniItem* GetItem(char* Key);
	CIniItem* GetItem(char* Section, char* Key);
	bool	ItemExists(char* Key, char* Section = NULL);

	char*	GetFirstSection();
	char*	GetNextSection(char* szLastSection);

	bool	GetBoolValue(char* Key, bool& ret, char* Section = NULL);
	bool	GetIntValue(char* Key, int& ret, char* Section = NULL);
	bool	GetFloatValue(char* Key, float& ret, char* Section = NULL);
	/* Get string representation of value. Dont save! may be freed*/
	bool    GetStringValue(char* Key, char*& ret, char* Section=NULL);
};  

#endif