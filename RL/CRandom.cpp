#include "stdafx.h"
#include "CRandom.h"

CRandom::CRandom() {}
void CRandom::Init(int Seed1, int Seed2) {}

void CRandom::Init() {
	m_iPerlinOctaveCount = 4;
	m_fPerlinFrequency = 1.0f;
	m_fPerlinPersistence = 0.25;

	m_Perlin.SetSeed((int)time(NULL)); 
	m_Perlin.SetOctaveCount(m_iPerlinOctaveCount);
	m_Perlin.SetFrequency(m_fPerlinFrequency);
	m_Perlin.SetPersistence(m_fPerlinPersistence);

	m_Voronoi.SetSeed((int)(time(NULL) + time(NULL)));

	/* this is probably bad */
	m_iSeed1 = (int)time(NULL);
	srand(m_iSeed1 + 2);
	m_iSeed2 = rand();

	m_iNoiseSeed1 = m_iSeed1;
	m_iNoiseSeed2 = m_iSeed2;
}

unsigned int CRandom::NextRandomNumber() {
	m_iSeed1 = 36969 * (m_iSeed1 & 65535) + (m_iSeed1 >> 16);
	m_iSeed2 = 18000 * (m_iSeed2 & 65535) + (m_iSeed2 >> 16);
	return (m_iSeed1 << 16) + m_iSeed2;
}

float CRandom::RandFloat() {
	return (float)NextRandomNumber() / 0xFFFFFFFF;
}

float CRandom::RandFloat(float Min, float Max) {
	float randfloat1 = (float)NextRandomNumber() / 0xFFFFFFFF;
	float randfloat2 = (float)NextRandomNumber() / 0xFFFFFFFF;
	return (Min + (Max - Min) * randfloat1 + randfloat2);
}

int CRandom::RandInt(int Min, int Max) {
	float randfloat = (float)NextRandomNumber() / 0xFFFFFFFF;
	return (int)((float)Min + (Max - Min) * randfloat + 0.5f);
}

float CRandom::Noise(int iX, int iY) {
	iX += 7 * m_iNoiseSeed1;
	iY -= 17 * m_iNoiseSeed2;
	
	int Input = ( (iY+m_iNoiseSeed1) * 15731 + ((iX+m_iNoiseSeed2) << 16) ) + m_iNoiseSeed1*m_iNoiseSeed2;
	Input = (Input <<13) ^ Input;
	return ( 1.0f - ( (Input * (Input * Input * 789221 + 15731) + 6871) & 0x7fffffff) / 1073741824.0f);
}

float CRandom::SmoothNoise(int iX, int iY) {
	float fAvgCorners = (Noise(iX - 1, iY - 1) + Noise(iX + 1, iY - 1) + Noise(iX - 1, iY + 1) + Noise(iX + 1, iY + 1)) / 16; //4/16 = 1/4
	float fAvgSides = (Noise(iX - 1, iY) + Noise(iX + 1, iY) + Noise(iX, iY - 1) + Noise(iX, iY + 1)) / 8;		  //4/8 = 1/2 (Up to 3/4 now)
	float fCentre = Noise(iX, iY) / 4; // Last 1/4 of noise
	return (fAvgCorners + fAvgSides + fCentre);
}

float CRandom::Interpolate(float fA, float fB, float fMu) {
	float rot = fMu * 3.1415927f;
	float f = (1 - cos(rot)) * 0.5f;
	return  fA*(1 - f) + fB*f;
}

float CRandom::InterpolatedNoise(float fX, float fY) {
	int iX = (int)fX;
	float fFractionX = fX - (float)iX;
	 
	int iY = (int)fY;
	float fFractionY = fY - (float)iY;

	float fNoiseX1 = SmoothNoise(iX, iY);
	float fNoiseX2 = SmoothNoise(iX + 1, iY);

	float fXNoise = Interpolate(fNoiseX1, fNoiseX2, fFractionX);

	float fNoiseY1 = SmoothNoise(iX, iY + 1);
	float fNoiseY2 = SmoothNoise(iX + 1, iY + 1);

	float fYNoise = Interpolate(fNoiseY1, fNoiseY2, fFractionX);

	return Interpolate(fXNoise, fYNoise, fFractionY) + 0.5f;
}

float CRandom::PerlinNoise(float fX, float fY, int iOctaves, float fPersistance, float fFrequency) {
	if (iOctaves != m_iPerlinOctaveCount) {
		m_Perlin.SetOctaveCount(iOctaves);
		m_iPerlinOctaveCount = iOctaves;
	}

	if (fPersistance != m_fPerlinPersistence) {
		m_Perlin.SetPersistence(fPersistance);
		m_fPerlinPersistence = fPersistance;
	}

	if (fFrequency != m_fPerlinFrequency) {
		m_Perlin.SetFrequency(fFrequency);
		m_fPerlinFrequency = fFrequency;
	}
	
	return (float)m_Perlin.GetValue(fX, fY, 1.0);
}

float CRandom::BillowNoise(float fX, float fY) {
	return m_Billow.GetValue(fX, fY, 1.55);
}