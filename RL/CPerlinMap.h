#ifndef __CPERLINMAP_H__
#define __CPERLINMAP_H__

#include "CGame.h"
#include "Vector.h"

#define PERLINMAP_WIDTH 1024
#define PERLINMAP_HEIGHT PERLINMAP_WIDTH

class CPerlinMap {
	int m_iWidth;
	int m_iHeight;

	float m_Tiles[PERLINMAP_WIDTH][PERLINMAP_HEIGHT];

public:
	CPerlinMap();
	~CPerlinMap();

	void Generate(CGame* pGame);
	void Draw(CGame* pGame, int iTileSize, Vector2i vPlayerLocation);

	/* Calculate a suitable spawn point, in tile indexes (not loc!) */
	Vector2i CalcSpawn();
};

#endif