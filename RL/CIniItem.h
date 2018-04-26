#pragma once 
struct CIniItem
{
	char*	m_szKey;
	char*	m_szValue;

	bool	MatchesKey(char* Key); /* Check if this this is the keyname supplied */
	bool	GetBool(); /* Get boolean representation of value */
	int		GetInt(); /* Get integer representation of value */
	float	GetFloat(); /* Get float representation of value */
	char*	GetString(); /* Gets string representation of value. !Don't save! may be free'd */
};