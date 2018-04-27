#include "stdafx.h"
#include "CWorldMap.h"
#include <iostream>
#include <sstream>

#define MAP_PERLIN_SPREAD 0.0035f // This is 1/UnitsPerBiome
#define MAP_PERLIN_OFFSET_RAINFALL 2003.0f
#define MAP_PERLIN_OFFSET_TEMP 185.8f
#define MAP_RIVERS_PER_TILE 0.00001f
#define MAP_EDGE_DISTANCE 150.0f

bool CWorldMap::InMapBounds(int x, int y) {
	return (x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight);
}

bool CWorldMap::InMapBounds(Vector2i vec) {
	return (vec.X >= 0 && vec.Y >= 0 && vec.X < m_iWidth && vec.Y < m_iHeight);
}

// Returns distance mask, circle from 1.0 at outer edges, 0.0f at centre
float EdgeDistanceGradient(int x, int y, int width, int height) {
	static float maxheight = Vector2f((width / 2.0f), (height / 2.0f)).Length() - MAP_EDGE_DISTANCE;
	float f = (Vector2f(x - (width / 2.0f), y - (height / 2.0f)).Length() / maxheight);

	if (f > 1.0f) 
		return 1.0f;
	return f * f;
}

void CWorldMap::GenRiver(CGame* pGame, int x, int y, int width) {
	float riverel = -1.0f;
	for (unsigned int j = 0; j < m_Terrain.size(); j++) {
		if (m_Terrain[j].m_bRiver) {
			riverel = m_Terrain[j].m_fElevation;
			break;
		}
	} 
	int lake_x = pGame->m_Random.RandInt(0, m_iWidth);
	int lake_y = pGame->m_Random.RandInt(0, m_iHeight);
	for (int i = lake_x; i < m_iWidth; i++) {
		for (int j = lake_y; j < m_iHeight; j++) {
			if (m_Tiles[i][j].m_Flags & F_Lake) {
				lake_x = i;
				lake_y = j;
				break;
			}
		}
	}
	// Didn't find lake from initial point, search backwards
	if (lake_x == m_iWidth && lake_y == m_iHeight) {
		for (int i = lake_x; i >= 0; i--) {
			for (int j = lake_y; j >= 0; j--) {
				if (m_Tiles[i][j].m_Flags & F_Lake) {
					lake_x = i;
					lake_y = j;
					break;
				}
			}
		}
	}
	// no lake on map
	if (lake_x == 0 && lake_y == 0)
		return;

	int step = 0;
	while ( !(x == lake_x && y == lake_y) ) {
		step++;
		int dir_x = 0;
		int dir_y = 0;

		if (x < lake_x)
			dir_x = 1;
		else if (x > lake_x)
			dir_x = -1;

		if (y < lake_y)
			dir_y = 1;
		else if (y > lake_y)
			dir_y = -1;

		x = x + dir_x;
		y = y + dir_y;
		 
		int diffx = abs(x - lake_x);
		int diffy = abs(y - lake_y);
		if (diffx + diffy > 15) {
			if (diffx > diffy) {
				//variate x
				float o = 2 * sin((step / 70.0f)*3.14f);
				x += (int)o;
			}
			else {
				//variate y
				float o = 2 * sin((step / 70.0f)*3.14f);
				y += (int)o;
			}
		}

		if (!InMapBounds(x, y))
			break;
		if (m_Tiles[x][y].m_Flags & F_Lake)
			break;
		int offw1 = pGame->m_Random.RandInt(0, 1);
		int offw2 = pGame->m_Random.RandInt(0, 1);
		int offw3 = pGame->m_Random.RandInt(0, 1);
		int offw4 = pGame->m_Random.RandInt(0, 1);
		for (int i = (-width)+offw1; i < width-offw2; i++) {
			for (int j = (-width)+offw3; j < width-offw4; j++) {
				if (InMapBounds(x + i, y + j)) {
					m_Tiles[x + i][y + j].m_fElevation = riverel;
				}
			} 
		}
	}
}

bool CWorldMap::Generate(CGame* pGame, bool bVisualiseElevation, bool bGenerateRivers) {
	// Re-seed random generator
	pGame->m_Random.Init();

	printf("Generating map data for %i tiles", m_iWidth * m_iHeight);

	float genstarttime = pGame->Time();

	pGame->m_Loading.ClearActivities();
	pGame->m_Loading.SetActivity("Generating map elevation", pGame);

	float fStepStart = genstarttime;

	for (int i = 0; i < m_iWidth; i++) {
		if (m_iWidth / 10 > 0 && i % (m_iWidth / 10) == 0) {
			printf(".");
			pGame->m_Loading.SetProgress(i / (float)m_iWidth, pGame);
		}

		for (int j = 0; j < m_iHeight; j++) {
			m_Tiles[i][j].m_Flags = 0;

			// Calculate randomized elevation of tile, masked by distance function from centre
			float fEdgeDist = EdgeDistanceGradient(i, j, m_iWidth, m_iHeight);
			m_Tiles[i][j].m_fElevation = ((pGame->m_Random.PerlinNoise(i * MAP_PERLIN_SPREAD, j * MAP_PERLIN_SPREAD, 3, 0.4f) + pGame->m_Random.BillowNoise(i * MAP_PERLIN_SPREAD, j * MAP_PERLIN_SPREAD)) / 2.0f) - fEdgeDist;

			// Calculate color variation of tile, range [0, 1]
			m_Tiles[i][j].m_fVar = pGame->m_Random.RandFloat(0.0f, 1.0f);

			// Add water flag to water tiles
			for (unsigned int k = 0; k < m_Terrain.size(); k++) {
				if (m_Tiles[i][j].m_fElevation <= m_Terrain[k].m_fElevation) {
					if (m_Terrain[k].m_eType == TYPE_WATER) {
						m_Tiles[i][j].m_Flags |= F_Lake;
						break;
					}
				}
			}
		}
	}
	printf(" Done in %.2f seconds.\n", pGame->m_Time.Time() - fStepStart);
	fStepStart = pGame->Time();

	if (bGenerateRivers) {
		int riverCount = (int)(MAP_RIVERS_PER_TILE * m_iWidth * m_iHeight);
		printf("Generating %i river paths", riverCount);
		pGame->m_Loading.SetActivity("Generating river paths", pGame);

		for (int i = 0; i < riverCount; i++) {
			if (riverCount / 10 > 0 && i % (riverCount / 10) == 0) {
				printf(".");
				pGame->m_Loading.SetProgress(i / (float)riverCount, pGame);
			}
			GenRiver(pGame, pGame->m_Random.RandInt(0, m_iWidth - 1), pGame->m_Random.RandInt(0, m_iHeight - 1), 6);
		}

		printf(" Done in %.2f seconds.\n", pGame->m_Time.Time() - fStepStart);
		fStepStart = pGame->Time();

		printf("Dredging riverbeds");
		pGame->m_Loading.SetActivity("Dredging riverbeds", pGame);
		for (int i = 0; i < m_iWidth; i++) {
			if (m_iWidth / 10 > 0 && i % (m_iWidth / 10) == 0) {
				printf(".");
				pGame->m_Loading.SetProgress(i / (float)m_iWidth, pGame);
			}

			float riverel = -1.0f;
			for (unsigned int j = 0; j < m_Terrain.size(); j++) {
				if (m_Terrain[j].m_bRiver) {
					riverel = m_Terrain[j].m_fElevation;
					break;
				}
			}

			for (int j = 0; j < m_iHeight; j++) {
				if (m_Tiles[i][j].m_Flags & F_River)
					m_Tiles[i][j].m_fElevation = riverel;
			}
		}
		printf(" Done in %.2f seconds.\n", pGame->m_Time.Time() - fStepStart);
		fStepStart = pGame->Time();
	}

	if (!bVisualiseElevation) {
		printf("Texturing tiles");
		pGame->m_Loading.SetActivity("Texturing tiles", pGame);

		for (int i = 0; i < m_iWidth; i++) {
			if (m_iWidth / 10 > 0 && i % (m_iWidth / 10) == 0) {
				pGame->m_Loading.SetProgress(i / (float)m_iWidth, pGame);
				printf(".");
			}

			for (int j = 0; j < m_iHeight; j++) {
				// Find terrain type for terrain elevation
				int tertype = m_Terrain.size() - 1;
				for (unsigned int k = 0; k < m_Terrain.size(); k++) {
					if (m_Tiles[i][j].m_fElevation <= m_Terrain[k].m_fElevation) {
						tertype = k;
						break;
					}
				}

				// Get texture ID
				if (m_Terrain[tertype].m_iTileCount > 1) {
					// Randomly select tile 
					int rndtile = pGame->m_Random.RandInt(0, m_Terrain[tertype].m_iTileCount - 1);
					m_Tiles[i][j].m_cTextureID = m_Terrain[tertype].m_iTextureID[rndtile];

				} else if (m_Terrain[tertype].m_iTileCount == 1) {
					m_Tiles[i][j].m_cTextureID = m_Terrain[tertype].m_iTextureID[0];
				}
			}
		}
		printf(" Done in %.2f seconds.\n", pGame->m_Time.Time() - fStepStart);
		fStepStart = pGame->Time();
	}

	// create map texture
	if (!m_MapTexture.create(WORLDMAP_WIDTH * WORLDMAP_TEXTURE_SIZE, WORLDMAP_HEIGHT * WORLDMAP_TEXTURE_SIZE)){
		printf("Creating map texture failed!\n");
		return false;
	}
	m_MapTexture.clear();

	printf("Rendering map as texture");
	pGame->m_Loading.SetActivity("Rendering world", pGame);
	
	if (bVisualiseElevation) {
		for (int i = 0; i < m_iWidth; i++) {
			if (m_iWidth / 10 > 0 && i % (m_iWidth / 10) == 0) {
				pGame->m_Loading.SetProgress(i / (float)m_iWidth, pGame);
				printf(".");
			}

			// Render elevation as shade of white 
			for (int j = 0; j < m_iHeight; j++) {
				char color = 255 - (char)(255 * ((1.0f + m_Tiles[i][j].m_fElevation) / 2.0f));
				pGame->m_Drawing.DrawSquareToTarget(&m_MapTexture, i*WORLDMAP_TEXTURE_SIZE, j*WORLDMAP_TEXTURE_SIZE,
					WORLDMAP_TEXTURE_SIZE, color, color, color, 255 - (unsigned char)(m_Tiles[i][j].m_fVar * 10));
			}
		}
		return true;
	}

	// Render each texture in sequence, so we minimise texture switching overhead
	for (char t = m_iStartTerrainID; t <= m_iLastTerrainID; t++) {
		for (int i = 0; i < m_iWidth; i++) {
			for (int j = 0; j < m_iHeight; j++) {
				if (m_Tiles[i][j].m_cTextureID == t) {
					pGame->m_Drawing.DrawSpriteToTarget(&m_MapTexture, i*WORLDMAP_TEXTURE_SIZE, j*WORLDMAP_TEXTURE_SIZE, WORLDMAP_TEXTURE_SIZE, 
						pGame->m_TextureManager.GetTexture(m_Tiles[i][j].m_cTextureID), 255 - (unsigned char)(m_Tiles[i][j].m_fVar * 10));
				}
			}
		}
		printf(".");
		pGame->m_Loading.SetProgress(t / (float)m_iLastTerrainID, pGame);
	}
	printf(" Done in %.2f seconds.\nMap generation complete (%f seconds total)\n", pGame->m_Time.Time() - fStepStart, pGame->m_Time.Time() - genstarttime);

	return true;
}

void CWorldMap::Draw(CGame* pGame, int iTileSize, Vector2i vPlayerLocation)
{
	pGame->m_Drawing.DrawSprite(&pGame->m_WindowManager, 0, 0,
		m_iWidth * iTileSize, m_iHeight* iTileSize, (sf::Texture*)&(m_MapTexture.getTexture()));
}
void CWorldMap::DrawMinimap(CGame* pGame, int iDrawSize, Vector2i vCentre, Vector2i vBlipLocation, int iWaterBorderSize, int iBorderSize) {
	if (iBorderSize > 0) {
		pGame->m_Drawing.DrawSquareCentred(&pGame->m_WindowManager, vCentre,
			iDrawSize, 255, 255, 255, 255);
		iDrawSize -= iBorderSize;
	}

	if (iWaterBorderSize > 0) {
		char waterTexture = 0;
		float deepestWater = 101.0f;
		for (unsigned int i = 0; i < m_Terrain.size(); i++) {
			if (m_Terrain[i].m_eType == TYPE_WATER && m_Terrain[i].m_fElevation < deepestWater) {
				deepestWater = m_Terrain[i].m_fElevation;
				waterTexture = (char)m_Terrain[i].m_iTextureID[0];
			}
		}

		if (deepestWater < 100.0f) {
			pGame->m_Drawing.DrawSpriteCentred(&pGame->m_WindowManager, vCentre.X, vCentre.Y,
				iDrawSize, iDrawSize, pGame->m_TextureManager.GetTexture(waterTexture));
			iDrawSize -= iWaterBorderSize;
		}
	}

	pGame->m_Drawing.DrawSpriteCentred(&pGame->m_WindowManager, vCentre.X, vCentre.Y,
		iDrawSize, iDrawSize, (sf::Texture*)&(m_MapTexture.getTexture()));

	Vector2f bliprelative = Vector2f((vBlipLocation.X / (float)m_iWidth)-0.5f, (vBlipLocation.Y / (float)m_iHeight)-0.5f);
	Vector2i blipdraw = Vector2i(vCentre.X + (int)(iDrawSize * bliprelative.X), vCentre.Y + (int)(iDrawSize * bliprelative.Y));
	pGame->m_Drawing.DrawRectangleCentred(&pGame->m_WindowManager, blipdraw, 4, 4, 255, 0, 0, 255);
}
Vector2i CWorldMap::CalcSpawn()
{
	return Vector2i(m_iHeight / 2, m_iWidth / 2);

	for (int i = 0; i < m_iWidth; i++) {
		for (int j = 0; j < m_iHeight; j++) {
			if (m_Tiles[i][j].m_Flags & F_DeepWater || m_Tiles[i][j].m_Flags & F_NotPassable)
				continue;
			return Vector2i(i, j);
		}
	}
	return Vector2i(1, 1); 
}
bool CWorldMap::Init(CGame* pGame)
{
	if (!pGame->m_IniReader.ParseFile(WORLDMAP_TERRAIN_FILE))
		return false;

	char* szTexture = 0;
	char* szTerrain = pGame->m_IniReader.GetFirstSection();
	while (szTerrain) {
		STerrain terrain;

		if (!pGame->m_IniReader.GetFloatValue("Depth", terrain.m_fElevation, szTerrain) || 
			!pGame->m_IniReader.GetBoolValue("Passable", terrain.m_bPassable, szTerrain) || 
			!pGame->m_IniReader.GetStringValue("Texture", szTexture, szTerrain)) {
			puts("Skipped terrain type with missing Depth, Passable or Texture properties.");
		}
		terrain.m_iTileCount = 0;

		if (!pGame->m_IniReader.GetBoolValue("River", terrain.m_bRiver, szTerrain)) {
			terrain.m_bRiver = false;
		}
		
		std::stringstream st(szTexture);
		std::string curtexture;
		while (std::getline(st, curtexture, ';')) {
			// Load ID for texture & insert
			int thisTextureID = pGame->m_TextureManager.GetID((char*)curtexture.c_str());
			terrain.m_iTextureID.push_back(thisTextureID);
			terrain.m_iTileCount++;

			// Record first/last texture IDs
			if (m_iStartTerrainID == -1) {
				m_iStartTerrainID = thisTextureID;
			}
			m_iLastTerrainID = thisTextureID;
		}

		// Terrain types require at least one texture
		if (terrain.m_iTileCount <= 0) {
			puts("Skipped terrain type with missing texture.");
			continue;
		}

		char* type;
		if (pGame->m_IniReader.GetStringValue("Type", type, szTerrain)) {

			if (!_strcmpi(type, "LAVA")) 
				terrain.m_eType = TYPE_LAVA;
			else if (!_strcmpi(type, "WATER")) 
				terrain.m_eType = TYPE_WATER;
			else 
				terrain.m_eType = TYPE_DEFAULT;

		} else {
			terrain.m_eType = TYPE_DEFAULT;
		}

		m_Terrain.push_back(terrain);
		szTerrain = pGame->m_IniReader.GetNextSection(szTerrain);
	}

	m_iHeight = WORLDMAP_HEIGHT;
	m_iWidth = WORLDMAP_WIDTH;

	// Check if textures were loaded
	if (m_iStartTerrainID == -1) {
		return false;
	}

	return true;
} 
CWorldMap::~CWorldMap()
{
}