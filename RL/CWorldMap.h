#pragma once
#define WORLDMAP_WIDTH 512
#define WORLDMAP_HEIGHT WORLDMAP_WIDTH
#define WORLDMAP_TEXTURE_SIZE 8

#include "CGame.h"
enum EWorldMapFlags
{
	F_Visible = 1 << 0,	/**/
	F_DeepWater = 1 << 2,	/**/
	F_NotPassable = 1 << 1,	/**/
	F_Transparent = 1 << 3,	/**/
	F_Lake = 1 << 4,
	F_River = 1 << 5,
	F_CountedRiver = 1 << 6,
};

enum ETileType
{
	/* Beach biome */
	TT_Water,
	TT_DeepWater,
	TT_Sand,
	TT_WetSand,
	TT_SandStone,

	/* Cold biome */
	TT_Ice,
	TT_SnowDirt,
	TT_SnowGrass,
	TT_SnowSand,

	/* Land biome */
	TT_Grass,
	TT_Dirt,

	/* Hot biome */
	TT_Lava,
	TT_Gravel,

	TT_Clay,

};
struct STileInfo
{
	float m_fElevation;
	float m_fRainfall;
	float m_fTemperature;

	ETileType CalcType();
};

struct SMapTile 
{
	float m_fVar;
	float m_fElevation;
	unsigned char m_cTextureID;
	unsigned char m_Flags;
};

class CWorldMap
{
	int m_iWidth;
	int m_iHeight;

	sf::RenderTexture m_MapTexture;

	SMapTile m_Tiles[WORLDMAP_WIDTH][WORLDMAP_HEIGHT];
	bool InMapBounds(int x, int y);
	bool InMapBounds(Vector2i vec);
	int RemoveRiverUnderSize(int x, int y, int minriversize);
public:
	CWorldMap();
	~CWorldMap();

	void Generate(CGame* pGame);
	void Draw(CGame* pGame, int iTileSize, Vector2i vPlayerLocation);
	void DrawMinimap(CGame* pGame, int iDrawSize, Vector2i vCentre, Vector2i vBlipLocation, bool bBorder);

	/* Calculate a suitable spawn point, in tile indexes (not loc!) */
	Vector2i CalcSpawn();
};