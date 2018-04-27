#ifndef __CWORLDMAP_H__
#define __CWORLDMAP_H__

#include "CGame.h"

#define WORLDMAP_WIDTH 1024
#define WORLDMAP_HEIGHT WORLDMAP_WIDTH
#define WORLDMAP_TEXTURE_SIZE 8
#define WORLDMAP_TERRAIN_FILE ".\\TERRAIN.ini"

enum EWorldMapFlags {
	F_Visible		= 1 << 0,
	F_DeepWater		= 1 << 2,	
	F_NotPassable	= 1 << 1,	
	F_Transparent	= 1 << 3,
	F_Lake			= 1 << 4,
	F_River			= 1 << 5,
	F_CountedRiver	= 1 << 6,
};

struct SMapTile {
	float m_fVar;
	float m_fElevation;
	unsigned char m_cTextureID;
	unsigned char m_Flags;
};

class CWorldMap {

	class CWorldChunk {
	};

	enum ETerrainType {
		TYPE_DEFAULT,
		TYPE_WATER,
		TYPE_LAVA,
	};

	struct STerrain {
		float	m_fElevation; 
		bool	m_bPassable;
		bool	m_bRiver;
		int		m_eType;
		int		m_iTileCount;
		std::vector<int> m_iTextureID;
	};

	std::vector< STerrain > m_Terrain;

	// first and last terrain texture IDs
	int m_iStartTerrainID = -1;
	int m_iLastTerrainID = -1;

	int m_iWidth;
	int m_iHeight;

	sf::RenderTexture m_MapTexture;
	SMapTile m_Tiles[WORLDMAP_WIDTH][WORLDMAP_HEIGHT];
	bool InMapBounds(int x, int y);
	bool InMapBounds(Vector2i vec);
	void GenRiver(CGame* pGame, int x, int y, int width);

public:
	bool Init(CGame* pGame);
	~CWorldMap();

	bool Regenerate(CGame* pGame, bool bVisualiseElevation = false, bool bGenerateRivers = false);
	bool Generate(CGame* pGame, bool bVisualiseElevation = false, bool bGenerateRivers = false);
	void Draw(CGame* pGame, int iTileSize, Vector2i vPlayerLocation);
	void DrawMinimap(CGame* pGame, int iDrawSize, Vector2i vCentre, Vector2i vBlipLocation, int iWaterBorderSize = 0, int iBorderSize = 0);

	/* Calculate a suitable spawn point, in tile indexes (not loc!) */
	Vector2i CalcSpawn();
};

#endif