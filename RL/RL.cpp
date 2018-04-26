#include "stdafx.h"
#include "CGame.h"
int main(int argc, char* argv[])
{
	CGame Game;
	Game.Initialise();

	while (!Game.ShouldQuit())
	{
		Game.Step();
		Game.HandleInput();
		Game.Draw();
	}

	return 0;
}

 