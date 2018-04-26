#include "stdafx.h"
#include "CWorldMap.h"

#define MAP_PERLIN_SPREAD 0.0015f // This value is 1/UnitsPerBiome
#define MAP_PERLIN_OFFSET_RAINFALL 2003.0f
#define MAP_PERLIN_OFFSET_TEMP 185.8f
#define MAP_RIVERS_PER_TILE 0.00005f // one river in 20,000 tiles

#define ICE_TEXTURE			".\\Graphics\\Terrain\\Ice.bmp"
#define WATER_TEXTURE		".\\Graphics\\Terrain\\Water.bmp"
#define DEEPWATER_TEXTURE	".\\Graphics\\Terrain\\DeepWater.bmp"
#define SAND_TEXTURE		".\\Graphics\\Terrain\\Sand.bmp"
#define WETSAND_TEXTURE		".\\Graphics\\Terrain\\WetSand.bmp"
#define GRASS_TEXTURE		".\\Graphics\\Terrain\\Grass.bmp"
#define SANDSTONE_TEXTURE	".\\Graphics\\Terrain\\SandStone.bmp"
#define SNOWDIRT_TEXTURE	".\\Graphics\\Terrain\\SnowDirt.bmp"
#define SNOWGRASS_TEXTURE	".\\Graphics\\Terrain\\SnowGrass.bmp"
#define SNOWSAND_TEXTURE	".\\Graphics\\Terrain\\SnowSand.bmp"
#define DIRT_TEXTURE		".\\Graphics\\Terrain\\Dirt.bmp"
#define LAVA_TEXTURE		".\\Graphics\\Terrain\\Lava.bmp"
#define GRAVEL_TEXTURE		".\\Graphics\\Terrain\\Gravel.bmp"
#define CLAY_TEXTURE		".\\Graphics\\Terrain\\Clay.bmp"


__inline ETileType CalcTileType(float fElevation, float fTemp)
{
//	if ( fTemp > -0.6 )
	{
		if (fElevation > -0.5f)
		{
			if (fElevation > 0.90f)
				return TT_Lava;
			if (fElevation > 0.70f)
				return TT_Gravel;
			else if (fElevation > 0.55f)
				return TT_Clay;
			else if (fElevation > 0.30f)
				return TT_Dirt;
			else if (fElevation > 0.00f)
				return TT_Grass;
			else if (fElevation > -0.20f)
				return TT_Sand;
			else if (fElevation > -0.35f)
				return TT_SandStone;
			else
				return TT_Sand;
		}
		else
		{
			if (fElevation > -0.6f)
				return TT_Water;
			else
				return TT_DeepWater;
		}
	}
	//else
	{
		if (fElevation > 0.9f)
			return TT_Clay;
		else if (fElevation > 0.7f)
			return TT_Gravel;
		else if (fElevation > 0.5f)
			return TT_SnowDirt;
		else if (fElevation > 0.2f)
			return TT_SnowGrass;
		else if (fElevation > 0.0f)
			return TT_SnowSand;
		else
			return TT_Ice;
	}
}

bool CWorldMap::InMapBounds(int x, int y)
{
	return (x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight);
}
bool CWorldMap::InMapBounds(Vector2i vec)
{
	return (vec.X >= 0 && vec.Y >= 0 && vec.X < m_iWidth && vec.Y < m_iHeight);
}

int CWorldMap::RemoveRiverUnderSize(int x, int y, int s) {
	if (!InMapBounds(x, y))
		return 0;
	
	if (!(m_Tiles[x][y].m_Flags & F_River))
		return 0;
	m_Tiles[x][y].m_Flags &= ~F_River;

	int size = 1 + (RemoveRiverUnderSize(x+1, y, s) + RemoveRiverUnderSize(x-1, y, s) + RemoveRiverUnderSize(x, y+1, s) + RemoveRiverUnderSize(x, y-1, s));
	if (size < s)
		return size;
	m_Tiles[x][y].m_Flags |= F_River;
	m_Tiles[x][y].m_Flags |= F_CountedRiver;
	return size;
}

void CWorldMap::Generate(CGame* pGame)
{
	float genstarttime = pGame->m_Time.Time();

	int icetex = pGame->m_TextureManager.GetID(ICE_TEXTURE);
	int watertex = pGame->m_TextureManager.GetID(WATER_TEXTURE);
	int sandtex = pGame->m_TextureManager.GetID(SAND_TEXTURE);
	int wetsandtex = pGame->m_TextureManager.GetID(WETSAND_TEXTURE);
	int grasstex = pGame->m_TextureManager.GetID(GRASS_TEXTURE);
	int sandstonetex = pGame->m_TextureManager.GetID(SANDSTONE_TEXTURE);
	int snowdirttex = pGame->m_TextureManager.GetID(SNOWDIRT_TEXTURE);
	int snowgrasstex = pGame->m_TextureManager.GetID(SNOWGRASS_TEXTURE);
	int snowsandtex = pGame->m_TextureManager.GetID(SNOWSAND_TEXTURE);
	int dirttex = pGame->m_TextureManager.GetID(DIRT_TEXTURE);
	int lavatex = pGame->m_TextureManager.GetID(LAVA_TEXTURE);
	int graveltex = pGame->m_TextureManager.GetID(GRAVEL_TEXTURE);
	int claytex = pGame->m_TextureManager.GetID(CLAY_TEXTURE);
	int deepwater = pGame->m_TextureManager.GetID(DEEPWATER_TEXTURE);

	if (deepwater > 255)
	{
		printf("[FATAL] Too many textures loaded before terrain texture.");
		exit(-1);
	}

	printf("Generating map data for %i tiles", m_iWidth * m_iHeight);
	pGame->m_Loading.SetActivity("Generating map elevation", pGame);

	ETileType thistiletype;
	long long total = 0;
	int count = 0;
	for (int i = 0; i < m_iWidth; i++)
	{
		if (i % (m_iWidth / 10) == 0) {
			printf(".");
			pGame->m_Loading.SetProgress(i / (float)m_iWidth, pGame);
		}

		for (int j = 0; j < m_iHeight; j++)
		{
			// Reset flags 
			m_Tiles[i][j].m_Flags = 0;

			// Calculate randomized elevation of tile 
			m_Tiles[i][j].m_fElevation = 0.5f * pGame->m_Random.PerlinNoise(i * MAP_PERLIN_SPREAD, j * MAP_PERLIN_SPREAD, 6, 0.4f, 2) + 0.49f * pGame->m_Random.BillowNoise(i * MAP_PERLIN_SPREAD, j * MAP_PERLIN_SPREAD);
			
			// Fuzz higher tiles
			if (m_Tiles[i][j].m_fElevation > -0.1f)
				m_Tiles[i][j].m_fElevation += 0.01f * pGame->m_Random.RandFloat(-1.0f, 1.0f);
			else
				m_Tiles[i][j].m_fElevation += 0.01f * pGame->m_Random.BillowNoise(i * MAP_PERLIN_SPREAD, j * MAP_PERLIN_SPREAD);

			//curinfo.m_fTemperature = pGame->m_Random.PerlinNoise((i*MAP_PERLIN_SPREAD)+MAP_PERLIN_OFFSET_TEMP, (j*MAP_PERLIN_SPREAD)+MAP_PERLIN_OFFSET_TEMP, 5, 0.5, 1);

			m_Tiles[i][j].m_fVar = 0.7f * pGame->m_Random.PerlinNoise(i * MAP_PERLIN_SPREAD, j * MAP_PERLIN_SPREAD, 1, 0.4f, 2) + pGame->m_Random.RandFloat(0.0, 0.3f);
		}
	}
	printf(" Done.\n");

	/* Generate rivers */
	int rivercount = (int)(MAP_RIVERS_PER_TILE * m_iWidth * m_iHeight);
	printf("Generating %i river paths", rivercount);
	pGame->m_Loading.SetActivity("Generating river paths", pGame);
	Vector2i curriverloc;
	for (int i = 0; i < rivercount; i++) {
		
		if (i % (rivercount / 10) == 0) {
			printf(".");
			pGame->m_Loading.SetProgress(i / (float)rivercount, pGame);
		}

		curriverloc = Vector2i(pGame->m_Random.RandInt(0, m_iWidth - 1), pGame->m_Random.RandInt(0, m_iHeight - 1));
		Vector2i nextriverloc = curriverloc;
		float minheight = 2.0f;
		while (InMapBounds(curriverloc)) {
			// underwater this tile & surrounding
			m_Tiles[curriverloc.X][curriverloc.Y].m_Flags |= F_River;

			curriverloc.X += 1;	// right
			if (!InMapBounds(curriverloc))
				break;
			if (!(m_Tiles[curriverloc.X][curriverloc.Y].m_Flags & F_River) && m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation < minheight) {
				//printf("right");
				nextriverloc = curriverloc;
				minheight = m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation;
			}
			m_Tiles[curriverloc.X][curriverloc.Y].m_Flags |= F_River;
			
			curriverloc.X -= 2;	// left
			if (!InMapBounds(curriverloc))
				break;
			if (!(m_Tiles[curriverloc.X][curriverloc.Y].m_Flags & F_River) && m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation < minheight) {
				//printf("left");
				nextriverloc = curriverloc;
				minheight = m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation;
			}
			m_Tiles[curriverloc.X][curriverloc.Y].m_Flags |= F_River;

			curriverloc.X += 1;	
			curriverloc.Y += 1; // up
			if (!InMapBounds(curriverloc))
				break;
			if (!(m_Tiles[curriverloc.X][curriverloc.Y].m_Flags & F_River) && m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation < minheight) {
				//printf("up");
				nextriverloc = curriverloc;
				minheight = m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation;
			}
			m_Tiles[curriverloc.X][curriverloc.Y].m_Flags |= F_River;

			curriverloc.Y -= 2; // down
			if (!InMapBounds(curriverloc))
				break;
			if (!(m_Tiles[curriverloc.X][curriverloc.Y].m_Flags & F_River) && m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation < minheight) {
				//printf("down");
				nextriverloc = curriverloc;
				minheight = m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation;
			}
			m_Tiles[curriverloc.X][curriverloc.Y].m_Flags |= F_River;

			// test -> diags.
			curriverloc.X += 1;	// bottom-right
			if (!InMapBounds(curriverloc))
				break;
			if (!(m_Tiles[curriverloc.X][curriverloc.Y].m_Flags & F_River) && m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation < minheight) {
				//printf("right");
				nextriverloc = curriverloc;
				minheight = m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation;
			}
			m_Tiles[curriverloc.X][curriverloc.Y].m_Flags |= F_River;

			curriverloc.X -= 2;	// bottom-left
			if (!InMapBounds(curriverloc))
				break;
			if (!(m_Tiles[curriverloc.X][curriverloc.Y].m_Flags & F_River) && m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation < minheight) {
				//printf("left");
				nextriverloc = curriverloc;
				minheight = m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation;
			}
			m_Tiles[curriverloc.X][curriverloc.Y].m_Flags |= F_River;

			curriverloc.Y += 2; // topleft
			if (!InMapBounds(curriverloc))
				break;
			if (!(m_Tiles[curriverloc.X][curriverloc.Y].m_Flags & F_River) && m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation < minheight) {
				//printf("up");
				nextriverloc = curriverloc;
				minheight = m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation;
			}
			m_Tiles[curriverloc.X][curriverloc.Y].m_Flags |= F_River;

			curriverloc.X += 2; // topright
			if (!InMapBounds(curriverloc))
				break;
			if (!(m_Tiles[curriverloc.X][curriverloc.Y].m_Flags & F_River) && m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation < minheight) {
				//printf("down");
				nextriverloc = curriverloc;
				minheight = m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation;
			}
			m_Tiles[curriverloc.X][curriverloc.Y].m_Flags |= F_River;

			// fix current locatioin
			curriverloc.X -= 1;
			curriverloc.Y -= 1;

			// dont flow uphill
			if ((m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation - m_Tiles[nextriverloc.X][nextriverloc.Y].m_fElevation) < -0.3f )
				break;
			if (m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation <= -0.51f)
				break;
			if (curriverloc == nextriverloc)
				break;

			//printf(" -> (%i, %i) (%f < %f)\n", curriverloc.X - nextriverloc.X, curriverloc.Y - nextriverloc.Y, m_Tiles[curriverloc.X][curriverloc.Y].m_fElevation, m_Tiles[nextriverloc.X][nextriverloc.Y].m_fElevation);
			curriverloc = nextriverloc;
			minheight = 100.0f;
		}
	}
	printf(" Done.\n"); 

	printf("Dredging riverbeds");
	pGame->m_Loading.SetActivity("Dredging riverbeds", pGame);
	for (int i = 0; i < m_iWidth; i++)
	{
		if (i % (m_iWidth / 10) == 0) {
			printf(".");
			pGame->m_Loading.SetProgress(i / (float)m_iWidth, pGame);
		}

		for (int j = 0; j < m_iHeight; j++)
		{
			if (m_Tiles[i][j].m_Flags & F_River)
				m_Tiles[i][j].m_fElevation = -0.5f;
		}
	}
	printf(" Done.\n");

	printf("Texturing tiles");
	pGame->m_Loading.SetActivity("Texturing tiles", pGame);
	for (int i = 0; i < m_iWidth; i++)
	{
		if (i % (m_iWidth / 10) == 0) {
			pGame->m_Loading.SetProgress(i / (float)m_iWidth, pGame);
			printf(".");
		}

		for (int j = 0; j < m_iHeight; j++)
		{
			thistiletype = CalcTileType(m_Tiles[i][j].m_fElevation, 0.0f);

			switch (thistiletype)
			{
			case TT_Water: m_Tiles[i][j].m_cTextureID = watertex; break;
			case TT_DeepWater: m_Tiles[i][j].m_cTextureID = deepwater; break;
			case TT_Sand: m_Tiles[i][j].m_cTextureID = sandtex; break;
			case TT_WetSand: m_Tiles[i][j].m_cTextureID = wetsandtex; break;
			case TT_SandStone: m_Tiles[i][j].m_cTextureID = sandstonetex; break;
			case TT_Ice: m_Tiles[i][j].m_cTextureID = icetex; break;
			case TT_SnowDirt: m_Tiles[i][j].m_cTextureID = snowdirttex; break;
			case TT_SnowGrass: m_Tiles[i][j].m_cTextureID = snowgrasstex; break;
			case TT_SnowSand: m_Tiles[i][j].m_cTextureID = snowsandtex; break;
			case TT_Grass: m_Tiles[i][j].m_cTextureID = grasstex; break;
			case TT_Dirt: m_Tiles[i][j].m_cTextureID = dirttex; break;
			case TT_Lava: m_Tiles[i][j].m_cTextureID = lavatex; break;
			case TT_Gravel: m_Tiles[i][j].m_cTextureID = graveltex; break;
			case TT_Clay:
			default:
				m_Tiles[i][j].m_cTextureID = graveltex; break;
			}
		}
	}
	printf(" Done.\n");
	
	printf("Rendering map as texture");
	pGame->m_Loading.SetActivity("Rendering world", pGame);
	for (int i = 0; i < m_iWidth; i++)
	{
		if (i % (m_iWidth / 10) == 0) {
			pGame->m_Loading.SetProgress(i / (float)m_iWidth, pGame);
			printf(".");
		}

		for (int j = 0; j < m_iHeight; j++)
		{
			pGame->m_Drawing.DrawSpriteToTarget(&m_MapTexture, i*WORLDMAP_TEXTURE_SIZE, j*WORLDMAP_TEXTURE_SIZE,
				WORLDMAP_TEXTURE_SIZE, WORLDMAP_TEXTURE_SIZE, pGame->m_TextureManager.GetTexture(m_Tiles[i][j].m_cTextureID));

			if (m_Tiles[i][j].m_fVar > 0.0f) {
				pGame->m_Drawing.DrawSquareToTarget(&m_MapTexture, Vector2i(i*WORLDMAP_TEXTURE_SIZE, j*WORLDMAP_TEXTURE_SIZE),
					WORLDMAP_TEXTURE_SIZE, 0, 0, 0, (unsigned char)(20.0 * m_Tiles[i][j].m_fVar));
			}
			else {
				pGame->m_Drawing.DrawSquareToTarget(&m_MapTexture, Vector2i(i*WORLDMAP_TEXTURE_SIZE, j*WORLDMAP_TEXTURE_SIZE),
					WORLDMAP_TEXTURE_SIZE, 255, 255, 255, (unsigned char)(-20.0* m_Tiles[i][j].m_fVar));
			}
		}
	}
	printf(" Done.\n");

	printf("Finished generating map (%f seconds)\n", pGame->m_Time.Time() - genstarttime);
}

void CWorldMap::Draw(CGame* pGame, int iTileSize, Vector2i vPlayerLocation)
{
	pGame->m_Drawing.DrawSprite(&pGame->m_WindowManager, 0, 0,
		m_iWidth * iTileSize, m_iHeight* iTileSize, (sf::Texture*)&(m_MapTexture.getTexture()));
}
void CWorldMap::DrawMinimap(CGame* pGame, int iDrawSize, Vector2i vCentre, Vector2i vBlipLocation, bool bBorder) {
	if ( bBorder)
		pGame->m_Drawing.DrawSquareCentred(&pGame->m_WindowManager, vCentre,
			iDrawSize + 2, 255, 255, 255, 255);

	pGame->m_Drawing.DrawSpriteCentred(&pGame->m_WindowManager, vCentre.X, vCentre.Y,
		iDrawSize, iDrawSize, (sf::Texture*)&(m_MapTexture.getTexture()));

	Vector2f bliprelative = Vector2f((vBlipLocation.X / (float)m_iWidth)-0.5f, (vBlipLocation.Y / (float)m_iHeight)-0.5f);
	Vector2i blipdraw = Vector2i(vCentre.X + (int)(iDrawSize * bliprelative.X), vCentre.Y + (int)(iDrawSize * bliprelative.Y));
	pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager, blipdraw, 4, 4, 255, 0, 0, 255);
}
Vector2i CWorldMap::CalcSpawn()
{
	return Vector2i(m_iHeight / 2, m_iWidth / 2);

	for (int i = 0; i < m_iWidth; i++)
	{
		for (int j = 0; j < m_iHeight; j++)
		{
			if (m_Tiles[i][j].m_Flags & F_DeepWater || m_Tiles[i][j].m_Flags & F_NotPassable)
				continue;
			return Vector2i(i, j);
		}
	}
	return Vector2i(1, 1); 
}
CWorldMap::CWorldMap()
{
	m_iHeight = WORLDMAP_HEIGHT;
	m_iWidth = WORLDMAP_WIDTH;

	m_MapTexture.create(WORLDMAP_WIDTH * WORLDMAP_TEXTURE_SIZE, WORLDMAP_HEIGHT * WORLDMAP_TEXTURE_SIZE);
} 
CWorldMap::~CWorldMap()
{
}