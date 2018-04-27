#ifndef __CDRAWINGMANAGER_H__
#define __CDRAWINGMANAGER_H__

#include "SFML/Graphics.hpp"
#include "CWindowManager.h"

#define GAME_FONT_FILE "DejaVuSans.ttf"

class CDrawingManager {
	sf::Font m_fFont;

public:
	CDrawingManager();
	~CDrawingManager();
	void Initialise();

	/* Uncentred drawing -> Position supplied is top-left */
	void DrawSprite(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawText(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, sf::Uint8 R, sf::Uint8 G, sf::Uint8 B, sf::Uint8 A);
	
	/* Centred drawing -> Position supplied is centre of where object is rendered */
	void DrawTextCentred(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, sf::Uint8 R, sf::Uint8 G, sf::Uint8 B, sf::Uint8 A);
	void DrawTextCentredX(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, sf::Uint8 R, sf::Uint8 G, sf::Uint8 B, sf::Uint8 A);
	void DrawSquareCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iSize, sf::Uint8 R, sf::Uint8 G, sf::Uint8 B, sf::Uint8 A);
	void DrawRectangleCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, sf::Uint8 R, sf::Uint8 G, sf::Uint8 B, sf::Uint8 A);
	void DrawOutlinedRectangleCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, sf::Uint8 R, sf::Uint8 G, sf::Uint8 B, sf::Uint8 A);

	void DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawSpriteCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, float fAlpha, sf::Texture* pTexture);
	void DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, float fAlpha, float fRotation, sf::Texture* pTexture);

	// Draw to render target, instead of only window
	void DrawSpriteToTargetCentred(sf::RenderTarget* t, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawSpriteToTargetCentred(sf::RenderTarget* t, int X, int Y, float fRotation, int iWidth, int iHeight, sf::Texture* pTexture);
	void DrawSpriteToTarget(sf::RenderTarget* pTarget, int X, int Y, sf::Vector2f& vSize, sf::Texture* pTexture, sf::Uint8 a);
	void DrawSpriteToTarget(sf::RenderTarget* pTarget, int X, int Y, int iSize, sf::Texture* pTexture, sf::Uint8 a);

	void DrawSquareToTargetCentred(sf::RenderTarget* t, Vector2i vPosition, int iSize, sf::Uint8 R, sf::Uint8 G, sf::Uint8 B, sf::Uint8 A);
	void DrawSquareToTarget(sf::RenderTarget* pTarget, int x, int y, sf::Vector2f& vSize, sf::Uint8 R, sf::Uint8 G,	sf::Uint8 B, sf::Uint8 A);
	void DrawSquareToTarget(sf::RenderTarget* pTarget, int x, int y, int iSize, sf::Uint8 R, sf::Uint8 G, sf::Uint8 B, sf::Uint8 A);
	void DrawSquareToTarget(sf::RenderTarget* pTarget, Vector2i vPosition, int iSize, sf::Uint8 R, sf::Uint8 G,	sf::Uint8 B, sf::Uint8 A);	
};

#endif