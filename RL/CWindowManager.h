#pragma once
#include "Vector.h"
#include <SFML/Graphics.hpp>
#include "CSettings.h"
#include "CInput.h"

class CWindowManager
{
	Vector2i m_vSize;
	bool m_bWindowCreated;
	bool m_bFocus;
	sf::RenderWindow* m_pWindow;
	bool m_bWindowClosed;

public:
	CWindowManager();
	~CWindowManager();
	void Create(CSettings* Settings, char* szTitle);
	void HandleEvents(CInput* pInput);
	sf::RenderWindow* GetWindow();

	/* Begin drawing */
	void Begin();
	/* Present scene */
	void Present();
	 
	bool WindowClosed();
	bool HasFocus();
	Vector2i GetScreenCentre();
	Vector2i GetScreenDimensions();

	void Draw(sf::Drawable* Item);
	void RestoreDefaultCamera();
	void MoveCamera(Vector2i CentrePos, int TileSize);
	void MoveCamera(Vector2f CentrePos);
};