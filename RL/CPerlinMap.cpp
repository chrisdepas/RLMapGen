#include "stdafx.h"
#include "CPerlinMap.h"

#define MAP_PERLIN_SPREAD 0.0015f // This value is 1/UnitsPerBiome
#define MAP_PERLIN_OFFSET_RAINFALL 2003.0f
#define MAP_PERLIN_OFFSET_TEMP 143.0f

void CPerlinMap::Generate(CGame* pGame)
{
	for (int i = 0; i < m_iWidth; i++)
	{
		for (int j = 0; j < m_iHeight; j++)
		{ 
			//m_Tiles[i][j] = 0.5f * pGame->m_Random.PerlinNoise(i * MAP_PERLIN_SPREAD, j * MAP_PERLIN_SPREAD, 6, 0.4, 2) + 0.5f * pGame->m_Random.BillowNoise(i * MAP_PERLIN_SPREAD, j * MAP_PERLIN_SPREAD);
			m_Tiles[i][j] = pGame->m_Random.PerlinNoise((i*MAP_PERLIN_SPREAD) + MAP_PERLIN_OFFSET_TEMP, (j*MAP_PERLIN_SPREAD) + MAP_PERLIN_OFFSET_TEMP, 5, 0.4f, 2);
			m_Tiles[i][j] += 1.0f;
			m_Tiles[i][j] /= 2.0f;
		}
	}
}

void CPerlinMap::Draw(CGame* pGame, int iTileSize, Vector2i vPlayerLocation)
{
	Vector2i TilesInWindow = (pGame->m_WindowManager.GetScreenCentre() / iTileSize); /* Pixels / Pixels per tile */

	/* leeway */
	TilesInWindow.X++;
	TilesInWindow.Y++;

	Vector2i PlayerLocTiles = vPlayerLocation / iTileSize; /* Player position in tiles */

	Vector2i DrawStart = PlayerLocTiles - TilesInWindow;
	Vector2i DrawEnd = PlayerLocTiles + TilesInWindow;

	if (DrawStart.X < 0)
		DrawStart.X = 0;
	if (DrawStart.Y < 0)
		DrawStart.Y = 0;
	if (DrawEnd.X > m_iWidth)
		DrawEnd.X = m_iWidth;
	if (DrawEnd.Y > m_iHeight)
		DrawEnd.Y = m_iHeight;

	/* Draw tiles */
	for (int i = DrawStart.X; i < DrawEnd.X; i++)
	{
		for (int j = DrawStart.Y; j < DrawEnd.Y; j++)
		{
			//pGame->m_Drawing.DrawSquare(&pGame->m_WindowManager, Vector2i(i*iTileSize, j*iTileSize),
			//	iTileSize, 255, 255, 255, (char)(255 * m_Tiles[i][j]));
		}
	}
}
Vector2i CPerlinMap::CalcSpawn()
{
	return Vector2i(m_iHeight / 2, m_iWidth / 2);
}
CPerlinMap::CPerlinMap()
{
	m_iHeight = PERLINMAP_HEIGHT;
	m_iWidth = PERLINMAP_WIDTH;
}
CPerlinMap::~CPerlinMap()
{
}