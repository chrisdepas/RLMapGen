#pragma once
#include <vector>
#include "CIniItem.h"

class CIniReader
{
	std::vector< CIniItem* > m_Items;
	void	ClearItems();
public:
	CIniReader();
	~CIniReader();
	CIniItem* GetItem(char* Key);
	bool	ItemExists(char* Key);
	bool	ParseFile(char* FileName);
	bool	GetBoolValue(char* Key, bool& ret);
	bool	GetIntValue(char* Key, int& ret);
	bool	GetFloatValue(char* Key, float& ret);
	/* Get string representation of value. Dont save! may be freed*/
	bool    GetStringValue(char* Key, char*& ret);
};