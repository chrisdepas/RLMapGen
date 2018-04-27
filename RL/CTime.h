#ifndef __CTIME_H__
#define __CTIME_H__

class CTime {
	double m_dClockRate;

public:
	CTime::CTime();
	float Time();
	double TimeDouble();
	
	void Pause();
};

#endif