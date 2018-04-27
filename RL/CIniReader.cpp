#include "stdafx.h"
#include "CIniReader.h"

CIniReader::CIniReader(){}
CIniReader::~CIniReader(){}

bool CIniReader::CIniSection::MatchesSection(char* Section) {
	return !_stricmp(Section, m_szSectionName);
}
char* CIniReader::GetFirstSection() {
	if (m_Sections.size() <= 0)
		return NULL;
	return m_Sections[0]->m_szSectionName;
}
char* CIniReader::GetNextSection(char* szSection) {
	if (!szSection)
		return NULL;
	bool bNextSection = false;
	for (unsigned int i = 0; i < m_Sections.size(); i++) {
		if (bNextSection)
			return m_Sections[i]->m_szSectionName;
		if (m_Sections[i]->MatchesSection(szSection))
			bNextSection = true;
	}
	return NULL;
}
CIniItem* CIniReader::GetItem(char* Key)
{
	// Check if item is in default section first
	for (unsigned int i = 0; i < m_DefaultItems.size(); i++)
	{
		if (!m_DefaultItems[i])
			continue;
		if (m_DefaultItems[i]->MatchesKey(Key))
			return m_DefaultItems[i];
	}

	// If not in default section, get first instance in other sections
	for (unsigned int i = 0; i < m_Sections.size(); i++)
	{
		for (unsigned int j = 0; j < m_Sections[i]->m_SectionItems.size(); j++) {
			if (m_Sections[i]->m_SectionItems[j]->MatchesKey(Key))
				return m_Sections[i]->m_SectionItems[j];
		}
	}

	return NULL;
}
CIniItem* CIniReader::GetItem(char* section, char* Key)
{
	if (!section)
		return GetItem(Key);
	for (unsigned int i = 0; i < m_Sections.size(); i++)
	{
		if (!m_Sections[i]->MatchesSection(section))
			continue;
		for (unsigned int j = 0; j < m_Sections[i]->m_SectionItems.size(); j++) {
			if (m_Sections[i]->m_SectionItems[j]->MatchesKey(Key))
				return m_Sections[i]->m_SectionItems[j];
		} 
	}
	return NULL;
}
bool CIniReader::ItemExists(char* Key, char* Section)
{
	if (!Section) {
		for (unsigned int i = 0; i < m_DefaultItems.size(); i++)
		{
			if (!m_DefaultItems[i])
				continue;
			if (m_DefaultItems[i]->MatchesKey(Key))
				return true;
		}
	}
	for (unsigned int i = 0; i < m_Sections.size(); i++)
	{
		if (Section && !m_Sections[i]->MatchesSection(Section))
			continue; 
		for (unsigned int j = 0; j < m_Sections[i]->m_SectionItems.size(); j++) {
			if (m_Sections[i]->m_SectionItems[j]->MatchesKey(Key))
				return true;
		}
	}
	return false;
}
void CIniReader::ClearItems()
{
	if (m_DefaultItems.empty() && m_Sections.empty())
		return;

	// Clear defaults
	for (unsigned int i = 0; i < m_DefaultItems.size(); i++)
	{
		if (m_DefaultItems[i]) {
			delete m_DefaultItems[i]->m_szKey;
			delete m_DefaultItems[i]->m_szValue;
			delete m_DefaultItems[i];
		}
	}
	m_DefaultItems.clear();

	// Clear sections
	for (unsigned int i = 0; i < m_Sections.size(); i++)
	{
		for (unsigned int j = 0; j < m_Sections[i]->m_SectionItems.size(); j++) {
			if (m_Sections[i]->m_SectionItems[j]) {
				delete m_Sections[i]->m_SectionItems[j]->m_szKey;
				delete m_Sections[i]->m_SectionItems[j]->m_szValue;
				delete m_Sections[i]->m_SectionItems[j];
			}
		}
	} 
	m_Sections.clear();
}
bool CIniReader::ParseFile(char* FileName)
{ 
	ClearItems();

	std::ifstream ini;
	ini.open(FileName);

	char thisline[1024];
	if (!ini.is_open() || ini.bad())
		return false;

	char* context = NULL;
	char* szCurSection = NULL;
	CIniSection* pCurSection = 0;

	while (ini.getline(thisline, 1024))
	{
		/* Skip comment lines*/
		if (thisline[0] == ';' || thisline[0] == '#')
		{
			continue;
		}
		
		/* Handle sections */
		if (thisline[0] == '[') {
			thisline[strlen(thisline) - 1] = 0;
			if (szCurSection == NULL || _strcmpi(szCurSection, thisline+1) != 0) {
				szCurSection = new char[strlen(thisline+1) + 1];
				memcpy(szCurSection, thisline+1, strlen(thisline+1) + 1);
				pCurSection = new CIniSection;
				pCurSection->m_szSectionName = szCurSection;
				m_Sections.push_back(pCurSection);
			}
			continue;
		}

		/* Get key string */
		char* key = strtok_s(thisline, "=", &context);
		if (!key)
			continue;
		if (!szCurSection && ItemExists(key))
			continue;
		if (szCurSection && ItemExists(key, szCurSection))
			continue;
		int keylen = strlen(key);

		/* Save copy of key to item */
		CIniItem* newitem = new CIniItem;
		newitem->m_szKey = new char[keylen + 1];
		memcpy(newitem->m_szKey, key, keylen);
		newitem->m_szKey[keylen] = 0;

		/* value of key */
		char* value = strtok_s(NULL, "=", &context);
		if (!value)
		{
			delete newitem->m_szKey;
			delete newitem;
			continue;
		}
		int valuelen = strlen(value);

		newitem->m_szValue = new char[valuelen + 1];
		memcpy(newitem->m_szValue, value, valuelen);
		newitem->m_szValue[valuelen] = 0;

		if (!pCurSection)
			m_DefaultItems.push_back(newitem);
		else
			pCurSection->m_SectionItems.push_back(newitem);
	}

	ini.close();

	return true;
}
bool CIniReader::GetBoolValue(char* Key, bool& ret, char* Section)
{
	CIniItem* item;
	if (Section)
		item = GetItem(Section, Key);
	else
		item = GetItem(Key);
	if (!item)
		return false;
	ret = item->GetBool();
	return true;
}
bool CIniReader::GetIntValue(char* Key, int& ret, char* Section)
{
	CIniItem* item;
	if (Section)
		item = GetItem(Section, Key);
	else
		item = GetItem(Key);
	if (!item)
		return false;
	ret = item->GetInt();
	return true;
}
bool CIniReader::GetFloatValue(char* Key, float& ret, char* Section)
{
	CIniItem* item;
	if (Section)
		item = GetItem(Section, Key);
	else
		item = GetItem(Key);
	if (!item)
		return false;
	ret = item->GetFloat();
	return true;
}
bool CIniReader::GetStringValue(char* Key, char*& ret, char* Section)
{
	CIniItem* item;
	if (Section)
		item = GetItem(Section, Key);
	else
		item = GetItem(Key);
	if (!item)
		return false;
	ret = item->GetString();
	return true;
}