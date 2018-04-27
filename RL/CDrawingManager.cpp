#include "stdafx.h"
#include "CDrawingManager.h"

CDrawingManager::CDrawingManager() {
}

CDrawingManager::~CDrawingManager() {
}

void CDrawingManager::Initialise() {
	m_fFont.loadFromFile(GAME_FONT_FILE); 
}

void CDrawingManager::DrawSprite(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture) {
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}

void CDrawingManager::DrawText(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A) {
	sf::Text text;
	text.setFont(m_fFont);
	text.setString(szText);
	text.setCharacterSize(iSize);
	text.setColor(sf::Color(R, G, B, A));
	text.setPosition((float)X, (float)Y);
	pWindowManager->Draw(&text);
}

void CDrawingManager::DrawTextCentred(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R,
	unsigned __int8 G, unsigned __int8 B, unsigned __int8 A) {
	sf::Text text;
	text.setFont(m_fFont);
	text.setString(szText);
	text.setCharacterSize(iSize);

	/* Centre the text */
	sf::FloatRect bound = text.getGlobalBounds();
	text.setOrigin(bound.left + bound.width / 2.0f, bound.top + bound.height / 2.0f);

	text.setColor(sf::Color(R, G, B, A));
	text.setPosition((float)X, (float)Y);
	pWindowManager->Draw(&text);
}

void CDrawingManager::DrawTextCentredX(CWindowManager* pWindowManager, char* szText, int X, int Y, int iSize, unsigned __int8 R,
	unsigned __int8 G, unsigned __int8 B, unsigned __int8 A) {
	sf::Text text;
	text.setFont(m_fFont);
	text.setString(szText);
	text.setCharacterSize(iSize);

	/* Centre the text */
	sf::FloatRect bound = text.getGlobalBounds();
	text.setOrigin(bound.left + bound.width / 2.0f, 0);

	text.setColor(sf::Color(R, G, B, A));
	text.setPosition((float)X, (float)Y);
	pWindowManager->Draw(&text);
}

void CDrawingManager::DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, float fAlpha, sf::Texture* pTexture) {
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f);

	/* Calculate integer alpha and set */
	int Alpha = (int)(255 * fAlpha);
	sp.setColor(sf::Color(255, 255, 255, Alpha));

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}

void CDrawingManager::DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, float fAlpha, float fRotation, sf::Texture* pTexture) {
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));
	
	sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f);
	sp.rotate(fRotation);

	/* Calculate integer alpha and set */
	int Alpha = (int)(255 * fAlpha);
	sp.setColor(sf::Color(255, 255, 255, Alpha));

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}

void CDrawingManager::DrawSpriteCentred(CWindowManager* pWindowManager, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture) {
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f); sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f);

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}

void CDrawingManager::DrawSpriteCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, sf::Texture* pTexture) {
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition((float)vPosition.X, (float)vPosition.Y);

	sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f);

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to window*/
	pWindowManager->Draw(&sp);
}

void CDrawingManager::DrawSquareCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A) {
	sf::RectangleShape sq(sf::Vector2f((float)iSize, (float)iSize));

	sq.setOrigin(iSize / 2.0f, iSize / 2.0f);
	sq.setPosition((float)vPosition.X, (float)vPosition.Y);
	sq.setFillColor(sf::Color(R, G, B, A));
	sq.setOutlineThickness(0.0f);

	pWindowManager->Draw(&sq);
}

void CDrawingManager::DrawRectangleCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A) {
	sf::RectangleShape sq(sf::Vector2f((float)iWidth, (float)iHeight));

	sq.setOrigin(iWidth/ 2.0f, iHeight/ 2.0f);
	sq.setPosition((float)vPosition.X, (float)vPosition.Y);
	sq.setFillColor(sf::Color(R, G, B, A));
	sq.setOutlineThickness(0.0f);

	pWindowManager->Draw(&sq);
}

void CDrawingManager::DrawOutlinedRectangleCentred(CWindowManager* pWindowManager, Vector2i vPosition, int iWidth, int iHeight, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A) {
	sf::RectangleShape sq(sf::Vector2f((float)iWidth, (float)iHeight));

	sq.setOrigin(iWidth/ 2.0f, iHeight/ 2.0f);
	sq.setPosition((float)vPosition.X, (float)vPosition.Y);
	sq.setFillColor(sf::Color(R, G, B, A));
	sq.setOutlineThickness(1.0f);

	pWindowManager->Draw(&sq);
}

void CDrawingManager::DrawSquareToTargetCentred(sf::RenderTarget* pTarget, Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A) {
	sf::RectangleShape sq(sf::Vector2f((float)iSize, (float)iSize));

	sq.setOrigin(iSize / 2.0f, iSize / 2.0f);
	sq.setPosition((float)vPosition.X, (float)vPosition.Y);
	sq.setFillColor(sf::Color(R, G, B, A));
	sq.setOutlineThickness(0.0f);

	pTarget->draw(sq);
}

void CDrawingManager::DrawSpriteToTargetCentred(sf::RenderTarget* pTarget, int X, int Y, int iWidth, int iHeight, sf::Texture* pTexture) {
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	sp.setOrigin(pTexture->getSize().x / 2.0f, pTexture->getSize().y / 2.0f);

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iWidth / f.x, (float)iHeight / f.y);
	sp.setScale(scale);

	/* Draw to target*/
	pTarget->draw(sp);
}

void CDrawingManager::DrawSquareToTarget(sf::RenderTarget* pTarget, Vector2i vPosition, int iSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A) {
	sf::RectangleShape sq(sf::Vector2f((float)iSize, (float)iSize));

	sq.setPosition((float)vPosition.X, (float)vPosition.Y);
	sq.setFillColor(sf::Color(R, G, B, A));

	pTarget->draw(sq);
}

void CDrawingManager::DrawSquareToTarget(sf::RenderTarget* pTarget, int x, int y, int iSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A) {
	sf::RectangleShape sq(sf::Vector2f((float)iSize, (float)iSize));

	sq.setPosition((float)x, (float)y);
	sq.setFillColor(sf::Color(R, G, B, A));

	pTarget->draw(sq);
}

void CDrawingManager::DrawSquareToTarget(sf::RenderTarget* pTarget, int x, int y, sf::Vector2f& vSize, unsigned __int8 R, unsigned __int8 G,
	unsigned __int8 B, unsigned __int8 A) {
	sf::RectangleShape sq(vSize);
	sq.setPosition((float)x, (float)y);
	sq.setFillColor(sf::Color(R, G, B, A));

	pTarget->draw(sq);
}

void CDrawingManager::DrawSpriteToTarget(sf::RenderTarget* pTarget, int X, int Y, int iSize, sf::Texture* pTexture, sf::Uint8 a) {
	if (!pTexture)
		return;

	/* Create a renderable sprite */
	sf::Sprite sp;
	sp.setTexture(*pTexture);
	sp.setPosition(sf::Vector2f((float)X, (float)Y));

	/* Scale to required dimensions */
	sf::Vector2u f = pTexture->getSize();
	sf::Vector2f scale((float)iSize / f.x, (float)iSize / f.y);
	sp.setScale(scale);
	sp.setColor(sf::Color(255, 255, 255, a));

	/* Draw to target*/
	pTarget->draw(sp);
}