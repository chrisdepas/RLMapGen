#include "stdafx.h"
#include "CIniItem.h"

bool CIniItem::MatchesKey(char* Key)
{
	return !_stricmp(Key, m_szKey);
}
bool	CIniItem::GetBool()
{
	if (!m_szValue)
		return false;
	if (!_stricmp("false", m_szValue) || !_stricmp("f", m_szValue) || !strcmp("0", m_szValue))
		return false;
	if (!_stricmp("true", m_szValue) || !_stricmp("t", m_szValue) || !strcmp("1", m_szValue))
		return true;
	return false; //?
}
int	CIniItem::GetInt()
{
	if (m_szValue)
		return strtol(m_szValue, NULL, 0);
	return 0;
}
float CIniItem::GetFloat()
{
	if (m_szValue)
		return (float)strtod(m_szValue, NULL);
	return 0.0f;
}
char* CIniItem::GetString()
{
	return m_szValue;
}