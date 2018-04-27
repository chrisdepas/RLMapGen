#include "stdafx.h"
#include "CTime.h"

CTime::CTime(){
	m_dClockRate = (double)CLOCKS_PER_SEC;
}

float CTime::Time() {
	return (float)(clock() / m_dClockRate);
}