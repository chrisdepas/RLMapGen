#pragma once
class CTime
{
	double m_dClockRate;
public:
	CTime::CTime();
	float Time();
	double TimeDouble();
	
	void Pause();
};
