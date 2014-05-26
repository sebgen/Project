/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
-------------------------------------------------------------------------
  History:
  2014-04-? 
  Created by Simon Johansson

*************************************************************************/

#include "Game.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	int retVal = 0;

	Game* game = new Game();

	if( FAILED( retVal = game->InitializeWindow( hInstance, nCmdShow ) ) )
		return retVal;

	if( FAILED( retVal = game->InitializeDirectX11() ) )
		return retVal;

	if( FAILED( retVal = game->InitializeInput() ) )
		return retVal;

	if( FAILED( retVal = game->InitializeGame() ) )
		return retVal;

	retVal = game->Run();

	delete game;

	return retVal;
}